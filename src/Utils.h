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

#ifndef _ROCKETCMS_UTILS_H
#define _ROCKETCMS_UTILS_H

#include <string>
#include <map>
#include <boost/filesystem/path.hpp>

namespace Utils {
	std::string readFile(const boost::filesystem::path& file);
	bool saveFile(const boost::filesystem::path& file, const std::string& text);
	void urlEncode(std::string& url);
	void urlDecode(std::string& url);
	std::string postDataGet(const std::string& httpContent);
	std::map<std::string, std::string> parseUrlQuery(const std::string& query);
};

#endif
