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

#ifndef _ROCKETCMS_CACHEMANAGER_H
#define _ROCKETCMS_CACHEMANAGER_H

#include <string>
#include <map>

class CacheManager {
public:
	static CacheManager& getInstance();

	bool has(const std::string& key) const;
	std::string get(const std::string& key) const;
	void set(const std::string& key, const std::string& value);
	void invalidate(const std::string& key);
	void clear();

private:
	CacheManager();
	CacheManager(CacheManager&);
	~CacheManager();

	void operator=(CacheManager&);

	std::map<std::string, std::string> _cache;
};

#endif
