/*
 * Copyright 2015 Oleg Morozenkov
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

#include "rcms/CacheManager.h"

#include "rcms/tools/ConfigTools.h"

using namespace std;
using namespace Poco;
using namespace Poco::Util;

CacheManager::~CacheManager() {
	if (_generalCache != nullptr) {
		delete _generalCache;
	}
	if (_privateCache != nullptr) {
		delete _privateCache;
	}
	_generalCache = _privateCache = nullptr;
}

CacheManager& CacheManager::getInstance() {
	static CacheManager result;
	return result;
}

void CacheManager::init() {
	if (ConfigTools::getConfig().getUInt("cache.general.size") > 0) {
		_generalCache = new LRUCache<string, string>(ConfigTools::getConfig().getUInt("cache.general.size"));
	}
	if (ConfigTools::getConfig().getUInt("cache.private.size") > 0) {
		_privateCache = new LRUCache<string, string>(ConfigTools::getConfig().getUInt("cache.private.size"));
	}
}

bool CacheManager::has(const string& key, CacheType cacheType) {
	switch (cacheType) {
		case CacheType::GENERAL:
			if (_generalCache != nullptr) {
				return _generalCache->has(key);
			}
		case CacheType::PRIVATE:
			if (_privateCache != nullptr) {
				return _privateCache->has(key);
			}
		default:
			return false;
	}
}
SharedPtr<string> CacheManager::get(const string& key, CacheType cacheType) {
	switch (cacheType) {
		case CacheType::GENERAL:
			if (_generalCache != nullptr) {
				return _generalCache->get(key);
			}
		case CacheType::PRIVATE:
			if (_privateCache != nullptr) {
				return _privateCache->get(key);
			}
		default:
			return SharedPtr<string>();
	}
}

void CacheManager::set(const string& key, const SharedPtr<string>& value, CacheType cacheType) {
	switch (cacheType) {
		case CacheType::GENERAL:
			if (_generalCache != nullptr) {
				_generalCache->update(key, value);
			}
			break;
		case CacheType::PRIVATE:
			if (_privateCache != nullptr) {
				_privateCache->update(key, value);
			}
			break;
	}
}

void CacheManager::remove(const string& key, CacheType cacheType) {
	switch (cacheType) {
		case CacheType::GENERAL:
			if (_generalCache != nullptr) {
				_generalCache->remove(key);
			}
			break;
		case CacheType::PRIVATE:
			if (_privateCache != nullptr) {
				_privateCache->remove(key);
			}
			break;
	}
}
