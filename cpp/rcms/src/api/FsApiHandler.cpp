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
	if (connection.methodName == "ls") {
		Path dirPath(ConfigTools::getPathFromConfig("fs.site.root", "fs.site.src"), connection.args.at("dir"));
		vector<string> files;
		File(dirPath).list(files);

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

		FsTools::loadFileToString(filePath);
		connection.responseMimeType = FsTools::getMimeType(filePath);
	} else if (connection.methodName == "getmeta") {
		Path filePath(ConfigTools::getPathFromConfig("fs.site.root", "fs.site.src"), connection.args.at("file"));

		string& key = connection.args.at("key");
		if (StringTools::startsWith(key, "_")) {
			if (key == "_isDir") {
				connection.response = File(filePath).isDirectory() ? "1" : "0";
			} else if (key == "_mimeType") {
				connection.response = FsTools::getMimeType(filePath);
			}
		} else {
			filePath.setExtension(filePath.getExtension() + ".meta.json");
			JSON::Parser parser;
			JSON::Object::Ptr jsonRoot = parser.parse(FsTools::loadFileToString(filePath)).extract<JSON::Object::Ptr>();
			connection.response = jsonRoot->get(key).convert<string>();
		}
	} else if (connection.methodName == "rm") {
		Path filePath(ConfigTools::getPathFromConfig("fs.site.root", "fs.site.src"), connection.args.at("file"));
		Path fileDestPath(ConfigTools::getPathFromConfig("fs.site.root", "fs.site.dst"), connection.args.at("file"));

		File(filePath).remove(true);
		File fileDestFile(fileDestPath);
		if (fileDestFile.exists()) {
			fileDestFile.remove(true);
		}

		PluginManager::getInstance().onFs(FsEvent(FsEvent::Type::RM, filePath));
	} else if (connection.methodName == "mv") {
		Path fromPath(ConfigTools::getPathFromConfig("fs.site.root", "fs.site.src"), connection.args.at("from"));
		Path toPath(ConfigTools::getPathFromConfig("fs.site.root", "fs.site.src"), connection.args.at("to"));

		File(fromPath).moveTo(toPath.toString());

		PluginManager::getInstance().onFs(FsEvent(FsEvent::Type::MV, fromPath, toPath));
	} else if (connection.methodName == "create") {
		Path filePath(ConfigTools::getPathFromConfig("fs.site.root", "fs.site.src"), connection.args.at("file"));

		filePath.setExtension(ConfigTools::getConfig().getString("fs.site.defaultFileExtention"));
		File(filePath).createFile();
	} else if (connection.methodName == "upload") {
		Path filePath(ConfigTools::getPathFromConfig("fs.site.root", "fs.site.src"), connection.args.at("file"));

		FileOutputStream stream(filePath.toString());
		stream << connection.postData;
		stream.close();
	} else if (connection.methodName == "setmeta") {
		if (StringTools::startsWith(connection.args.at("key"), "_")) {
			connection.responseCode = 405;
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
		Path fileDestPath(ConfigTools::getPathFromConfig("fs.site.root", "fs.site.dst"), connection.args.at("file"));

		File(filePath).moveTo(fileDestPath.toString());

		PluginManager::getInstance().onFs(FsEvent(FsEvent::Type::PUBLISH, filePath));
	}
}
