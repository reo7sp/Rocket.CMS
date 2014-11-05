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

#include "ConfigManager.h"

#include "Log.h"

#include <sstream>
#include <exception>
#include <rapidjson/document.h>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

using namespace std;
using namespace rapidjson;
namespace fs = boost::filesystem;

ConfigManager::ConfigManager() {
}

ConfigManager::~ConfigManager() {
}

ConfigManager& ConfigManager::getInstance() {
	static ConfigManager instance;
	return instance;
}

void ConfigManager::reload() {
	Log::info("Parsing config");

	fs::path file("res/config.json");
	if (fs::exists(file)) {
		fs::ifstream input(file);
		stringstream buffer;
		string line;
		while (getline(input, line)) {
			buffer << line;
		}
		input.close();

		Document document;
		document.Parse(buffer.str().c_str());

		if (document.HasMember("sitePath")) {
			_sitePath = document["sitePath"].GetString();
		} else {
			Log::warn("\"sitePath\" value doesn't exist in config");
		}
		if (document.HasMember("port")) {
			_port = document["port"].GetString();
		} else {
			Log::warn("\"port\" value doesn't exist in config");
		}
	} else {
		Log::warn("Config file doesn't exist");
	}

	Log::info("Config has been parsed");
}
