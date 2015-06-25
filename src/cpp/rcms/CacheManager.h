/*
 * Copyright 2015 Reo_SP
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
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

namespace CacheManager {

static inline Poco::LRUCache<std::string, std::string>& getFsCache() {
    static Poco::LRUCache<std::string, std::string> cache(Poco::Util::Application::instance().config().getUInt("cache.fs.size"));
    return cache;
}

}

#endif //ROCKET_CMS_CACHEMANAGER_H
