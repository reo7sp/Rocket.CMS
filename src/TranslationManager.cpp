/*
 * Copyright 2014 Reo_SP
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "TranslationManager.h"

#include "Log.h"
#include "Utils.h"
#include "ConfigManager.h"

#include <sstream>
#include <rapidjson/document.h>
#include <boost/filesystem/path.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/case_conv.hpp>

using namespace std;
using namespace rapidjson;
using namespace boost::algorithm;
namespace fs = boost::filesystem;

TranslationManager::TranslationManager() {
}

TranslationManager::~TranslationManager() {
}

TranslationManager& TranslationManager::getInstance() {
	static TranslationManager instance;
	return instance;
}

void TranslationManager::reload() {
	Log::info("Parsing tranlations");

	string json = Utils::readFile(fs::path("res/translations/" + ConfigManager::getInstance().getLang() + ".json"));
	if (!json.empty()) {
		Document document;
		document.Parse(json.c_str());

		for (Value::ConstMemberIterator iter = document.MemberBegin(); iter != document.MemberEnd(); ++iter) {
			_data[iter->name.GetString()] = iter->value.GetString();
		}

		Log::info("Translations has been parsed");
	} else {
		Log::warn("Translations file doesn't exist");
	}
}

string TranslationManager::get(const string& key) const {
	try {
		return _data.at(key);
	} catch (out_of_range& e) {
		return key;
	}
}

void TranslationManager::translate(string& text) const {
	for (auto& item : _data) {
		replace_all(text, "%" + to_upper_copy(item.first) + "STR%", item.second);
	}
}
