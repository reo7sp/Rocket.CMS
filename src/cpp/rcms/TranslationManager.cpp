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

#include <iostream>
#include <map>

#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/JSON.h>

#include "TranslationManager.h"
#include "tools/FsTools.h"


std::string TranslationManager::get(std::string key) {
    auto pos = this->_store.find(key);
    if (pos == this->_store.end()) {
        return "Key not found";
    } else {
        std::string val = pos->second;
        return val;
    }
}

void TranslationManager::parseJSON(Poco::JSON::Object& obj) {
    for (Poco::JSON::Object::ConstIterator it = obj.begin(), end = obj.end(); it != end; ++it) {
        _store.insert(std::pair<std::string, std::string>(it->first, it->second.toString()));
    }
}

void TranslationManager::parseJSONFile(const std::string& f) {
    std::string lFile = FsTools::loadFileToString(f);

    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = Poco::JSON::Parser().parse(lFile);

    Poco::JSON::Object::Ptr pObj = result.extract<Poco::JSON::Object::Ptr>();
    parseJSON(*pObj);
}
