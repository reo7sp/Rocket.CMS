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

#include "rcms/api/WebguiApiHandler.h"

#include <Poco/Path.h>
#include <Poco/File.h>
#include <Poco/DirectoryIterator.h>

#include "rcms/CacheManager.h"
#include "rcms/TranslationManager.h"
#include "rcms/tools/FsTools.h"
#include "rcms/tools/ConfigTools.h"
#include "rcms/tools/StringTools.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;

WebguiApiHandler::WebguiApiHandler() : AbstractApiHandler("webgui") {
}

void WebguiApiHandler::handleRequest(ApiConnection& connection) const {
	if (connection.methodName == "getfile") {
		string fileStr = connection.args.at("file");
		Path filePath = ConfigTools::getPathFromConfig("fs.cms.root");
		if (fileStr.find("plugins/") == 0) {
			vector<string> fileNameParts = StringTools::split(fileStr, '/');
			filePath.append("plugins");
			filePath.append(fileNameParts.at(1));
			filePath.append("webgui");
			for (size_t i = 2, count = fileNameParts.size(); i < count; ++i) {
				filePath.append(fileNameParts[i]);
			}
		} else {
			filePath.append("webgui");
			filePath.append(fileStr);
		}
		File fileFile(filePath);
		if (fileFile.exists()) {
			if (fileFile.isFile()) {
				SharedPtr<string> cached = CacheManager::getInstance().get("webgui-getfile-" + fileStr, CacheManager::PRIVATE);
				if (cached) {
					connection.response = *cached;
				} else {
					connection.response = FsTools::loadFileToString(filePath);
					if (fileStr == "app.js" || fileStr == "style.css") {
						connection.response += concatPluginFiles(fileStr);
					}
					CacheManager::getInstance().set("webgui-getfile-" + fileStr, connection.response, CacheManager::PRIVATE);
				}
				connection.responseMimeType = FsTools::getMimeTypeOfFile(filePath);
			} else {
				connection.responseCode = 403;
				connection.response = "Requested file is not a file";
			}
		} else {
			connection.responseCode = 404;
			connection.response = "Not found";
		}
	} else if (connection.methodName == "getstr") {
		connection.response = TranslationManager::getInstance().get(connection.args.at("file"));
	}
}

string WebguiApiHandler::concatPluginFiles(const std::string& pluginFileName) const {
	vector<Path> files;
	File pluginsDirFile(Path(ConfigTools::getPathFromConfig("fs.cms.root"), "plugins"));
	if (pluginsDirFile.exists() && pluginsDirFile.isDirectory()) {
		for (DirectoryIterator iter(pluginsDirFile), end; iter != end; ++iter) {
			Path filePath = iter.path();
			filePath.append("webgui");
			filePath.append(pluginFileName);

			File fileFile(filePath);
			if (fileFile.exists() && fileFile.isFile()) {
				files.push_back(filePath);
			}
		}
	}
	return FsTools::concat(files);
}
