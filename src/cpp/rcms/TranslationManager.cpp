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

void TranslationManager::set(std::string key, std::string value) {
    this->store.insert(std::pair<std::string, std::string>(key, value));
}

std::string TranslationManager::get(std::string key) {
    auto pos = this->store.find(key);
    if (pos == this->store.end()) {
        return "error";
    } else {
        std::string val = pos->second;
        return val;
    }
}

void TranslationManager::parseJSON(Object& obj, const std::string& name = "") {
    for (Object::ConstIterator it = obj.begin(), end = obj.end(); it != end; ++it) {
        this->set(it->first, it->second.toString());
    }
}

void TranslationManager::parseJSONFile(std::string f) {
    Poco::Path fp(f);
    std::string lFile = FsTools::loadFileToString(fp.toString());

    std::cout << lFile << std::endl;

    JSON::Parser parser;
    Dynamic::Var result = Parser().parse(lFile);

    Object::Ptr pObj = result.extract<Object::Ptr>();
    this->parseJSON(*pObj);
}
