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

class CacheManager {

public:
	static CacheManager& getInstance();

	Poco::LRUCache<std::string, std::string>& getGeneralCache();
	Poco::LRUCache<std::string, std::string>& getPrivateCache();

private:
	CacheManager() { }
	CacheManager(const CacheManager&) = delete;
	CacheManager& operator=(const CacheManager&) = delete;
};

#endif //ROCKET_CMS_CACHEMANAGER_H
