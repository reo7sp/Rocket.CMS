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
#ifndef ROCKET_CMS_TRANSLATIONMANAGER_H
#define ROCKET_CMS_TRANSLATIONMANAGER_H

#include <Poco/Path.h>
#include <Poco/File.h>
#include "tools/FsTools.h"

#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <iostream>
#include <map>

using namespace Poco;
using namespace Poco::JSON;

class TranslationManager {
public:
    TranslationManager() { }
    void parseJSON(Object& obj, const std::string& name = "");
    void parseJSONFile(std::string f);
    void set(std::string key, std::string value);
    std::string get(std::string key);
private:
    std::map<std::string, std::string> store;
};

#endif
