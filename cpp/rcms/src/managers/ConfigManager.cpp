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

#include "rcms/managers/ConfigManager.h"

using namespace std;

pair<bool, vector<string>> ConfigManager::checkAndFixConfig() {
	vector<string> log;
	bool isOk = true;

	if (!config().has("fs.site.root")) {
		log.push_back("fs.site.root must be set in the config");
		isOk = false;
	}
	if (!config().has("fs.site.src")) {
		log.push_back("fs.site.src must be set in the config");
		isOk = false;
	}
	if (!config().has("fs.site.dst")) {
		log.push_back("fs.site.dst must be set in the config");
		isOk = false;
	}
	if (!config().has("fs.site.defaultFileExtention")) {
		log.push_back("fs.site.defaultFileExtention is set to \"md\"");
		config().setString("fs.site.defaultFileExtention", "md");
	}
	if (!config().has("fs.cms.root")) {
		log.push_back("fs.cms.root must be set in the config");
		isOk = false;
	}
	if (!config().has("web.lang")) {
		log.push_back("web.lang is set to \"en\"");
		config().setString("web.lang", "en");
	}
	if (!config().has("web.port")) {
		log.push_back("web.port is set to 23307");
		config().setUInt("web.port", 23307);
	}
	if (!config().has("web.auth.enabled")) {
		log.push_back("web.auth.enabled is set to false");
		config().setBool("web.auth.enabled", false);
	} else if (config().getBool("web.auth.enabled")) {
		if (!config().has("web.auth.user")) {
			log.push_back("web.auth.user must be set in the config");
			isOk = false;
		}
		if (!config().has("web.auth.passhash")) {
			log.push_back("web.auth.passhash must be set in the config");
			isOk = false;
		}
	}
	if (!config().has("debug")) {
		config().setBool("debug", false);
	}

	return isOk;
};
