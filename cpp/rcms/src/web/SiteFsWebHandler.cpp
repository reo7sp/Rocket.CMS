/*
 * Copyright 2015 Oleg Morozenkov
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	 http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "rcms/web/SiteFsWebHandler.h"

#include <fstream>

#include <Poco/String.h>
#include <Poco/URI.h>
#include <Poco/Path.h>
#include <Poco/File.h>
#include <rcms/tools/FsTools.h>
#include <Poco/JSON/Stringifier.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>

#include "rcms/events/FsEvent.h"
#include "rcms/tools/StringTools.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;

void SiteFsWebHandler::handleRequestInternal(HTTPServerRequest& request, HTTPServerResponse& response) {
	static const unsigned long fileNameStart = string("/api/site/fs/").length();

	const URI uri = URI(request.getURI());
	const string uriPath = uri.getPath();
	const auto uriQuery = uri.getQueryParameters();

	const bool isAboutMeta = StringTools::endsWith(uriPath, "/meta");
	const bool hasNonCrudActions = !uriQuery.empty();

	const unsigned long fileNameEnd = isAboutMeta ? uriPath.find('/', fileNameStart) : uriPath.length();
	const string fileName = replace(uriPath.substr(fileNameStart, fileNameEnd - fileNameStart), "../", "");
	const Path filePath(_core.getConfigManager().extractPath("fs.site.root", "fs.site.src"), fileName + (isAboutMeta ? ".meta.json" : ""));
	File file(filePath);

	if (isAboutMeta) {
		const string metaKey = uriPath.substr(fileNameEnd + 1);

		if (request.getMethod() == "GET") {
			if (!file.exists()) {
				sendError(response, HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
				return;
			}
			if (StringTools::startsWith(metaKey, "_")) {
				const Path filePath(_core.getConfigManager().extractPath("fs.site.root", "fs.site.src"), fileName);
				File file(filePath);
				handleGetSpecialMeta(response, file, filePath, metaKey);
			} else {
				handleGetMeta(response, file, filePath, metaKey);
			}
		} else if (request.getMethod() == "PUT") {
			if (!file.exists() || !file.isFile()) {
				sendError(response, HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
				return;
			}
			if (StringTools::startsWith(metaKey, "_")) {
				sendError(response, HTTPResponse::HTTPStatus::HTTP_METHOD_NOT_ALLOWED, "Keys with leading '_' are reserved for special use. Values which correspond to this key can't be edited");
				return;
			}
			handleSetMeta(response, file, filePath, metaKey);
		} else if (request.getMethod() == "DELETE") {
			if (!file.exists() || !file.isFile()) {
				sendError(response, HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
				return;
			}
			if (StringTools::startsWith(metaKey, "_")) {
				sendError(response, HTTPResponse::HTTPStatus::HTTP_METHOD_NOT_ALLOWED, "Keys with leading '_' are reserved for special use. Values which correspond to this key can't be edited");
				return;
			}
			handleDeleteMeta(response, file, filePath, metaKey);
		} else {
			sendError(response, HTTPResponse::HTTPStatus::HTTP_METHOD_NOT_ALLOWED);
		}
	} else if (hasNonCrudActions) {
		if (!file.exists() || !file.isFile()) {
			sendError(response, HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
			return;
		}
		if (request.getMethod() == "POST") {
			bool isPublish = false;
			bool isMove = false;
			string fromFileName;
			Path fromPath;

			for (const pair<string, string>& item : uriQuery) {
				if (item.first == "do") {
					if (item.second == "publish") {
						isPublish = true;
					}
				} else if (item.first == "using" && item.second == "mv") {
					isMove = true;
				} else if (item.first == "from") {
					fromFileName = item.second;
					fromPath = Path(_core.getConfigManager().extractPath("fs.site.root", "fs.site.src"), item.second);
				}
			}

			if (isPublish) {
				handlePublish(response, file, filePath, fileName);
			} else if (isMove) {
				handleMove(response, File(fromPath), fromPath, file, filePath, fromFileName, fileName);
			} else {
				sendError(response, HTTPResponse::HTTPStatus::HTTP_METHOD_NOT_ALLOWED);
			}
		} else {
			sendError(response, HTTPResponse::HTTPStatus::HTTP_METHOD_NOT_ALLOWED);
		}
	} else {
		if (request.getMethod() == "GET") {
			if (!file.exists()) {
				sendError(response, HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
				return;
			}
			if (file.isFile()) {
				handleGetFile(response, file, filePath);
			} else {
				handleLs(response, file, filePath);
			}
		} else if (request.getMethod() == "POST") {
			handleCreate(response, file, filePath);
		} else if (request.getMethod() == "PUT") {
			handleUpload(response, file, filePath);
		} else if (request.getMethod() == "DELETE") {
			if (!file.exists()) {
				sendError(response, HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
				return;
			}
			handleDelete(response, file, filePath, fileName);
		} else {
			sendError(response, HTTPResponse::HTTPStatus::HTTP_METHOD_NOT_ALLOWED);
		}
	}
}

void SiteFsWebHandler::handleLs(HTTPServerResponse& response, File& file, const Path& filePath) {
	string result;

	vector<string> files;
	file.list(files);
	sort(files.begin(), files.end());
	for (string item : files) {
		if (StringTools::endsWith(item, ".meta.json") || StringTools::startsWith(item, ".")) {
			continue;
		}
		result += item;
		result += "\r\n";
	}
	trimRightInPlace(result);

	response.setContentType("text/plain");
	response.setContentLength(result.length());
	response.send() << result;
}

void SiteFsWebHandler::handleGetFile(HTTPServerResponse& response, File& file, const Path& filePath) {
	ifstream stream(filePath.toString(), ios::ate);

	response.setContentType(FsTools::getMimeTypeOfFile(filePath));
	response.setContentLength(stream.tellg());
	stream.seekg(0, stream.beg);
	response.send() << stream;
}

void SiteFsWebHandler::handleCreate(HTTPServerResponse& response, File& file, const Path& filePath) {
	if (file.isFile()) {
		Path newFilePath = filePath;
		File newFile = file;
		if (filePath.getExtension().empty()) {
			newFilePath.setExtension(_core.getConfigManager().getConfig().getString("fs.site.defaultFileExtention"));
			newFile = File(filePath);
		}
		File(newFilePath).createDirectories();
		newFile.createFile();
	} else {
		file.createDirectories();
	}
	_core.getPluginManager().onFs(FsEvent(FsEvent::Type::NEW, filePath));
}

void SiteFsWebHandler::handleUpload(HTTPServerResponse& response, File& file, const Path& filePath) {
	if (!FsTools::writeStringToFile(filePath, connection.postData)) {
		sendError(response, HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, "Can't save file");
	}
}

void SiteFsWebHandler::handleDelete(HTTPServerResponse& response, File& file, const Path& filePath, const string& rawFilePath) {
	const Path publishedFilePath(_core.getConfigManager().extractPath("fs.site.root", "fs.site.dst"), rawFilePath);
	File publishedFile(publishedFilePath);

	file.remove(true);
	if (publishedFile.exists()) {
		publishedFile.remove(true);
	}
	_core.getPluginManager().onFs(FsEvent(FsEvent::Type::RM, filePath));
}

void SiteFsWebHandler::handleMove(HTTPServerResponse& response, File& fromFile, const Path& fromFilePath, File& file, const Path& filePath, const string& rawFromFilePath, const string& rawFilePath) {
	const Path publishedFromFilePath(_core.getConfigManager().extractPath("fs.site.root", "fs.site.dst"), rawFromFilePath);
	const Path publishedFilePath(_core.getConfigManager().extractPath("fs.site.root", "fs.site.dst"), rawFilePath);

	fromFile.moveTo(filePath.toString());
	File(publishedFromFilePath).moveTo(publishedFilePath.toString());
	_core.getPluginManager().onFs(FsEvent(FsEvent::Type::MV, fromPath, toPath));
}

void SiteFsWebHandler::handlePublish(HTTPServerResponse& response, File& file, const Path& filePath, const string& rawFilePath) {
	const Path publishedFilePath(_core.getConfigManager().extractPath("fs.site.root", "fs.site.dst"), rawFilePath);
	// TODO: templates
	file.copyTo(publishedFilePath.toString());
	_core.getPluginManager().getInstance().onFs(FsEvent(FsEvent::Type::PUBLISH, filePath));
}

void SiteFsWebHandler::handleGetSpecialMeta(HTTPServerResponse& response, File& file, const Path& filePath, const string& key) {
	string result;
	if (key == "_isDir") {
		result = File(filePath).isDirectory() ? "1" : "0";
	} else if (key == "_mimeType") {
		result = FsTools::getMimeTypeOfFile(filePath);
	}

	response.setContentType("text/plain");
	response.setContentLength(result.length());
	response.send() << result;
}

void SiteFsWebHandler::handleGetMeta(HTTPServerResponse& response, File& file, const Path& filePath, const string& key) {
	string result;
	JSON::Parser parser;
	JSON::Object::Ptr jsonRoot = parser.parse(FsTools::loadFileToString(filePath)).extract<JSON::Object::Ptr>();
	if (!jsonRoot->has(key)) {
		sendError(response, HTTPResponse::HTTP_NOT_FOUND, "No value for requested key in meta file exists");
		return;
	}
	result = jsonRoot->get(key).convert<string>();

	response.setContentType("text/plain");
	response.setContentLength(result.length());
	response.send() << result;
}

void SiteFsWebHandler::handleSetMeta(HTTPServerResponse& response, File& file, const Path& filePath, const string& key, const string& value) {
	Dynamic::Var jsonRootVar = JSON::Parser().parse(FsTools::loadFileToString(filePath));
	JSON::Object::Ptr jsonRoot = jsonRootVar.extract<JSON::Object::Ptr>();
	jsonRoot->set(key, value);

	ofstream stream(filePath.toString());
	JSON::Stringifier::stringify(jsonRootVar, stream, 4);
	stream.close();
}

void SiteFsWebHandler::handleDeleteMeta(HTTPServerResponse& response, File& file, const Path& filePath, const string& key) {
	Dynamic::Var jsonRootVar = JSON::Parser().parse(FsTools::loadFileToString(filePath));
	JSON::Object::Ptr jsonRoot = jsonRootVar.extract<JSON::Object::Ptr>();
	jsonRoot->remove(key);

	ofstream stream(filePath.toString());
	JSON::Stringifier::stringify(jsonRootVar, stream, 4);
	stream.close();
}