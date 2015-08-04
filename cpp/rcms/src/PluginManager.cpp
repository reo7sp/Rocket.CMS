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

#include "rcms/PluginManager.h"

#include <Poco/File.h>
#include <Poco/SharedLibrary.h>
#include <Poco/DirectoryIterator.h>

#include "rcms/tools/ConfigTools.h"
#include "rcms/tools/LogTools.h"

using namespace std;
using namespace Poco;

PluginManager& PluginManager::getInstance() {
	static PluginManager result;
	return result;
}

void PluginManager::load() {
	File pluginsDirFile(Path(ConfigTools::getPathFromConfig("fs.cms.root"), "plugins"));
	if (!pluginsDirFile.exists() || !pluginsDirFile.isDirectory()) {
		return;
	}
	for (DirectoryIterator iter(pluginsDirFile), end; iter != end; ++iter) {
		string pluginLibName;
		for (DirectoryIterator innerIter(pluginsDirFile); innerIter != end; ++innerIter) {
			if (innerIter.path().getExtension() == "so" || innerIter.path().getExtension() == "dll" || innerIter.path().getExtension() == "dylib") {
				pluginLibName = innerIter.path().toString();
				break;
			}
		}
		if (pluginLibName.empty()) {
			continue;
		}
		try {
			SharedLibrary pluginLib(pluginLibName);
			typedef rcms_Plugin* (*MainFunc)();
			MainFunc func = (MainFunc) pluginLib.getSymbol("rcms_plug_main");
			_plugins.push_back(func());
		} catch (NotFoundException& e) {
			LogTools::getLogger().error("Error while loading %s. %s", pluginLibName, e.displayText());
		}
	}
}

bool PluginManager::onPreInit() {
	bool result = true;
	for (rcms_Plugin*& item : _plugins) {
		if (!item->onPreInit(&_core)) {
			result = false;
		}
	}
	return result;
}

bool PluginManager::onInit() {
	bool result = true;
	for (rcms_Plugin*& item : _plugins) {
		if (!item->onInit(&_core)) {
			result = false;
		}
	}
	return result;
}

void PluginManager::onDeinit() {
	for (rcms_Plugin*& item : _plugins) {
		item->onDeinit(&_core);
	}
}

void PluginManager::onFs(const FsEvent& event) {
	rcms_FsEvent cEvent;
	cEvent.type = event.type;
	cEvent.file1Path = (char*) event.file1Path.c_str();
	cEvent.file2Path = (char*) event.file2Path.c_str();
	for (rcms_Plugin*& item : _plugins) {
		item->onFsEvent(&_core, &cEvent);
	}
}

bool PluginManager::onApi(ApiConnection& connection) {
	rcms_StrPair* cArgs = (rcms_StrPair*) malloc(connection.args.size() * sizeof(rcms_StrPair));
	size_t i = 0;
	for (auto& item : connection.args) {
		rcms_StrPair cArg;
		cArg.first = (char*) item.first.c_str();
		cArg.second = (char*) item.second.c_str();
		cArgs[i++] = cArg;
	}
	rcms_ApiConnection cConnection;
	cConnection.handler = (char*) connection.handlerName.c_str();
	cConnection.method = (char*) connection.methodName.c_str();
	cConnection.args = cArgs;
	cConnection.argsCount = connection.args.size();
	cConnection.postData = (char*) connection.postData.c_str();

	if (_plugins.empty()) {
		return false;
	}

	bool result = true;
	for (rcms_Plugin*& item : _plugins) {
		if (strcmp(item->apiHandlerName, connection.handlerName.c_str()) == 0) {
			if (!item->handleApi(&_core, &cConnection)) {
				result = false;
			}
		}
	}
	return result;
}
