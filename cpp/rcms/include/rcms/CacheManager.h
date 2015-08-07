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

#ifndef ROCKET_CMS_CACHEMANAGER_H
#define ROCKET_CMS_CACHEMANAGER_H

#include <string>

#include <Poco/Util/Application.h>
#include <Poco/LRUCache.h>
#include <Poco/SharedPtr.h>

class CacheManager {

public:
	enum CacheType {
		GENERAL,
		PRIVATE
	};

	static CacheManager& getInstance();

	void init();
	bool has(const std::string& key, CacheType cacheType = CacheType::GENERAL);
	Poco::SharedPtr<std::string> get(const std::string& key, CacheType cacheType = CacheType::GENERAL);
	void set(const std::string& key, const Poco::SharedPtr<std::string>& value, CacheType cacheType = CacheType::GENERAL);
	void remove(const std::string& key, CacheType cacheType = CacheType::GENERAL);

	inline void set(const std::string& key, const std::string& value, CacheType cacheType = CacheType::GENERAL) {
		set(key, Poco::SharedPtr<std::string>(new std::string(value)), cacheType);
	}

private:
	CacheManager() { }
	CacheManager(const CacheManager&) = delete;
	CacheManager& operator=(const CacheManager&) = delete;
	~CacheManager();

	Poco::LRUCache<std::string, std::string>* _generalCache = nullptr;
	Poco::LRUCache<std::string, std::string>* _privateCache = nullptr;
};

#endif //ROCKET_CMS_CACHEMANAGER_H
