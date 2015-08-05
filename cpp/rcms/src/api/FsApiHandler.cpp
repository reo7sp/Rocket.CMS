/*
 * Copyright 2015 Oleg Morozenkov
 * Copyright 2015 Alexander Rizaev
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

#include "rcms/api/FsApiHandler.h"

#include <Poco/Util/Application.h>
#include <Poco/File.h>
#include <Poco/FileStream.h>
#include <Poco/JSON/Parser.h>

#include "rcms/FsEvent.h"
#include "rcms/PluginManager.h"
#include "rcms/tools/FsTools.h"
#include "rcms/tools/StringTools.h"
#include "rcms/tools/ConfigTools.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::Util;

FsApiHandler::FsApiHandler() : AbstractApiHandler("fs") {
}

void FsApiHandler::handleRequest(ApiConnection& connection) const {
	map<string, string>::iterator iter = connection.args.find("file");
	if (iter != connection.args.end() && iter->second.front() == '/') {
		iter->second.erase(0, 1);
	}
	iter = connection.args.find("dir");
	if (iter != connection.args.end() && iter->second.front() == '/') {
		iter->second.erase(0, 1);
	}

	if (connection.methodName == "ls") {
		File dirFile(Path(ConfigTools::getPathFromConfig("fs.site.root", "fs.site.src"), connection.args.at("dir")));
		if (!dirFile.exists() || !dirFile.isDirectory()) {
			connection.responseCode = 404;
			connection.response = "Not found";
			return;
		}

		vector<string> files;
		dirFile.list(files);
		for (string item : files) {
			if (StringTools::endsWith(item, ".meta.json") || StringTools::startsWith(item, ".")) {
				continue;
			}
			connection.response += item;
			connection.response += "\r\n";
		}
		trimRightInPlace(connection.response);
	} else if (connection.methodName == "getfile") {
		Path filePath(ConfigTools::getPathFromConfig("fs.site.root", "fs.site.src"), connection.args.at("file"));
		File fileFile(filePath);
		if (!fileFile.exists()) {
			connection.responseCode = 404;
			connection.response = "Not found";
			return;
		}

		connection.response = FsTools::loadFileToString(filePath);
		connection.responseMimeType = FsTools::getMimeTypeOfFile(filePath);
	} else if (connection.methodName == "getmeta") {
		Path filePath(ConfigTools::getPathFromConfig("fs.site.root", "fs.site.src"), connection.args.at("file"));
		File fileFile(filePath);
		if (!fileFile.exists()) {
			connection.responseCode = 404;
			connection.response = "Not found";
			return;
		}

		string& key = connection.args.at("key");
		if (StringTools::startsWith(key, "_")) {
			if (key == "_isDir") {
				connection.response = File(filePath).isDirectory() ? "1" : "0";
			} else if (key == "_mimeType") {
				connection.response = FsTools::getMimeTypeOfFile(filePath);
			}
		} else {
			filePath.setExtension(filePath.getExtension() + ".meta.json");
			fileFile = File(filePath);
			if (!fileFile.exists()) {
				connection.responseCode = 404;
				connection.response = "No meta file exists";
				return;
			}
			JSON::Parser parser;
			JSON::Object::Ptr jsonRoot = parser.parse(FsTools::loadFileToString(filePath)).extract<JSON::Object::Ptr>();
			if (!jsonRoot->has(key)) {
				connection.responseCode = 404;
				connection.response = "No value for requested key in meta file exists";
				return;
			}
			connection.response = jsonRoot->get(key).convert<string>();
		}
	} else if (connection.methodName == "rm") {
		Path filePath(ConfigTools::getPathFromConfig("fs.site.root", "fs.site.src"), connection.args.at("file"));
		File fileFile(filePath);
		Path fileDestPath(ConfigTools::getPathFromConfig("fs.site.root", "fs.site.dst"), connection.args.at("file"));
		File fileDestFile(fileDestPath);

		if (fileDestFile.exists()) {
			fileFile.remove(true);
		} else {
			connection.responseCode = 404;
			connection.response = "Not found";
			return;
		}
		if (fileDestFile.exists()) {
			fileDestFile.remove(true);
		}

		PluginManager::getInstance().onFs(FsEvent(FsEvent::Type::RM, filePath));
	} else if (connection.methodName == "mv") {
		Path fromPath(ConfigTools::getPathFromConfig("fs.site.root", "fs.site.src"), connection.args.at("from"));
		File fromFile(fromPath);
		Path toPath(ConfigTools::getPathFromConfig("fs.site.root", "fs.site.src"), connection.args.at("to"));

		if (!fromFile.exists()) {
			connection.responseCode = 404;
			connection.response = "Not found";
			return;
		}
		fromFile.moveTo(toPath.toString());

		PluginManager::getInstance().onFs(FsEvent(FsEvent::Type::MV, fromPath, toPath));
	} else if (connection.methodName == "create") {
		Path filePath(ConfigTools::getPathFromConfig("fs.site.root", "fs.site.src"), connection.args.at("file"));
		File fileFile(filePath);

		if (filePath.getExtension().empty()) {
			filePath.setExtension(ConfigTools::getConfig().getString("fs.site.defaultFileExtention"));
		}
		fileFile.createFile();

		PluginManager::getInstance().onFs(FsEvent(FsEvent::Type::NEW, filePath));
	} else if (connection.methodName == "upload") {
		Path filePath(ConfigTools::getPathFromConfig("fs.site.root", "fs.site.src"), connection.args.at("file"));

		if (!FsTools::writeStringToFile(filePath, connection.postData)) {
			connection.responseCode = 500;
			connection.response = "Can't save file";
		}
	} else if (connection.methodName == "setmeta") {
		if (StringTools::startsWith(connection.args.at("key"), "_")) {
			connection.responseCode = 405;
			connection.response = "Values with keys with leading '_' are reserved for special use. They can't be edited";
			return;
		}

		Path filePath(ConfigTools::getPathFromConfig("fs.site.root", "fs.site.src"), connection.args.at("file") + ".meta.json");

		Dynamic::Var jsonRootVar = JSON::Parser().parse(FsTools::loadFileToString(filePath));
		JSON::Object::Ptr jsonRoot = jsonRootVar.extract<JSON::Object::Ptr>();
		jsonRoot->set(connection.args.at("key"), connection.args.at("value"));

		ofstream stream(filePath.toString());
		JSON::Stringifier::stringify(jsonRootVar, stream, 4);
		stream.close();
	} else if (connection.methodName == "publish") {
		Path filePath(ConfigTools::getPathFromConfig("fs.site.root", "fs.site.src"), connection.args.at("file"));
		File fileFile(filePath);
		Path fileDestPath(ConfigTools::getPathFromConfig("fs.site.root", "fs.site.dst"), connection.args.at("file"));

		if (!fileFile.exists()) {
			connection.responseCode = 404;
			connection.response = "Not found";
			return;
		}
		fileFile.moveTo(fileDestPath.toString());

		PluginManager::getInstance().onFs(FsEvent(FsEvent::Type::PUBLISH, filePath));
	}
}
