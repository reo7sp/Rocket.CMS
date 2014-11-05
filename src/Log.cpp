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

#include "Log.h"

#include <stdio.h>

using namespace std;

void Log::log(const string& message, const string& mode) {
	time_t rawtime = time(nullptr);
	tm tm = *localtime(&rawtime);
	printf("[%s] %02d:%02d:%02d %s\n", mode.c_str(), tm.tm_hour, tm.tm_min, tm.tm_sec, message.c_str());
}

void Log::debug(const string& message) {
	log(message, "DEBUG");
}

void Log::info(const string& message) {
	log(message, "INFO");
}

void Log::warn(const string& message) {
	log(message, "WARN");
}

void Log::error(const string& message) {
	log(message, "ERROR");
}
