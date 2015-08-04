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
		if (fileStr == "plugins.css") {
			connection.response = concatPluginFiles("style.css");
			connection.responseMimeType = FsTools::getMimeType("css");
		} else if (fileStr == "plugins.js") {
			connection.response = concatPluginFiles("app.js");
			connection.responseMimeType = FsTools::getMimeType("js");
		} else {
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
			if (File(filePath).exists()) {
				connection.response = FsTools::loadFileToString(filePath);
				connection.responseMimeType = FsTools::getMimeTypeOfFile(filePath);
			} else {
				connection.responseCode = 404;
				connection.response = "Not found";
			}
		}
	}
}

string WebguiApiHandler::concatPluginFiles(const std::string& pluginFileName) const {
	SharedPtr<string> cached = CacheManager::getInstance().getPrivateCache().get("plugins-" + pluginFileName + "-concat");
	if (cached) {
		return *cached;
	}

	vector<Path> files;

	File pluginsDirFile(Path(ConfigTools::getPathFromConfig("fs.cms.root"), "plugins"));
	if (pluginsDirFile.exists() && pluginsDirFile.isDirectory()) {
		for (DirectoryIterator iter(pluginsDirFile), end; iter != end; ++iter) {
			Path filePath = pluginsDirFile.path();
			filePath.append(iter.path());
			filePath.append("webgui");
			filePath.append(pluginFileName);
			if (File(filePath).exists()) {
				files.push_back(filePath);
			}
		}
	}

	string result = FsTools::concat(files);
	CacheManager::getInstance().getPrivateCache().add("plugins-" + pluginFileName + "-concat", result);
	return result;
}
