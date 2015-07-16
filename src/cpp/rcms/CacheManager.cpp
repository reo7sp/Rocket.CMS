/*
 * Copyright 2015 Oleg Morozenkov,
 *		   2015 Alexander Rizaev
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

#include "CacheManager.h"

using namespace std;
using namespace Poco;
using namespace Poco::Util;

CacheManager& CacheManager::getInstance() {
	static CacheManager result;
	return result;
}

Poco::LRUCache<std::string, std::string>& CacheManager::getGeneralCache() {
	static LRUCache<string, string> cache(Application::instance().config().getUInt("cache.general.size"));
	return cache;
}

Poco::LRUCache<std::string, std::string>& CacheManager::getPrivateCache() {
	static LRUCache<string, string> cache(Application::instance().config().getUInt("cache.private.size"));
	return cache;
}
