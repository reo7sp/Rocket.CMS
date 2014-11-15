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

#include "CacheManager.h"

#include <stdexcept>

using namespace std;

CacheManager& CacheManager::get() {
	static CacheManager instance;
	return instance;
}

bool CacheManager::hasString(const string& key) const {
	return getString(key).empty();
}

string CacheManager::getString(const string& key) const {
	if (key.empty()) return "";
	try {
		return _cache.at(key);
	} catch (out_of_range& e) {
		return "";
	}
}

void CacheManager::setString(const string& key, const string& value) {
	if (key.empty()) return;
	_cache[key] = value;
}

void CacheManager::removeString(const string& key) {
	_cache.erase(key);
}

void CacheManager::clearAll() {
	_cache.clear();
}
