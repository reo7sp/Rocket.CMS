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

#include "rcms/CacheManager.h"
#include "rcms/tools/FsTools.h"
#include "rcms/tools/ConfigTools.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;

WebguiApiHandler::WebguiApiHandler() : AbstractApiHandler("webgui") {
}

void WebguiApiHandler::handleRequest(ApiConnection& connection) const {
	if (connection.methodName == "getfile") {
		Path filePath(ConfigTools::getPathFromConfig("fs.cms.root"), connection.args.at("file"));

		if (connection.args.at("file") == "plugins.css") {
			connection.response = concatPluginFiles("style.css");
		} else if (connection.args.at("file") == "plugins.js") {
			connection.response = concatPluginFiles("app.js");
		} else {
			connection.response = FsTools::loadFileToString(filePath);
		}
		connection.responseMimeType = FsTools::getMimeType(filePath);
	} else if (connection.methodName == "getstr") {
		// TODO
		connection.responseCode = 501;
	}
}

string WebguiApiHandler::concatPluginFiles(const std::string& pluginFileName) const {
	SharedPtr<string> cached = CacheManager::getInstance().getPrivateCache().get("plugins-" + pluginFileName + "-concat");
	if (!cached) {
		return *cached;
	}

	vector<Path> files;
	vector<string> pluginDirs;
	Path pluginsRootDir(ConfigTools::getPathFromConfig("fs.cms.root"), "plugins");
	File(pluginsRootDir).list(pluginDirs);
	for (string& pluginDir : pluginDirs) {
		files.push_back(Path(pluginsRootDir, pluginDir + Path::separator() + "webgui" + Path::separator() + pluginFileName));
	}
	string result = FsTools::concat(files);
	CacheManager::getInstance().getPrivateCache().add("plugins-" + pluginFileName + "-concat", result);
	return result;
}
