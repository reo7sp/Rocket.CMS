/*
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

#include "TranslationManager.h"

#include <Poco/DirectoryIterator.h>
#include <Poco/Util/Application.h>
#include <Poco/JSON/Parser.h>

#include "rcms/tools/FsTools.h"

using namespace std;
using namespace Poco;
using namespace Poco::Util;

TranslationManager& TranslationManager::getInstance() {
    static TranslationManager result;
    return result;
}

void TranslationManager::load() {
    Path rcmsTranslationsDirPath(Application::instance().config().getString("fs.root"), "translations");
	Path pluginsDirPath(Application::instance().config().getString("fs.root"), "plugins");
    string lang = Application::instance().config().getString("cms.lang");

	loadFile(Path(rcmsTranslationsDirPath, lang + ".json"));
    for (DirectoryIterator iter(pluginsDirPath), end; iter != end; ++iter) {
		loadFile(Path(iter.path(), lang + ".json"));
    }
}

const string& TranslationManager::get(const string& key) const {
	auto iter = _store.find(key);
    if (iter == _store.end()) {
        return key;
    } else {
        return *iter->second;
    }
}

void TranslationManager::loadFile(const Path& filePath) {
	if (!File(filePath).exists()) {
		return;
	}
	JSON::Object::Ptr jsonRoot = JSON::Parser().parse(FsTools::loadFileToString(filePath)).extract<JSON::Object::Ptr>();
	for (JSON::Object::ConstIterator iter = jsonRoot->begin(), end = jsonRoot->end(); iter != end; ++iter) {
		_store[iter->first] = iter->second.toString();
	}
}
