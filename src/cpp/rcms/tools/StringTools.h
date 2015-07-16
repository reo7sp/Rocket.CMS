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

#ifndef ROCKET_CMS_STRINGTOOLS_H
#define ROCKET_CMS_STRINGTOOLS_H

#include <vector>
#include <string>
#include <sstream>

namespace StringTools {

bool startsWith(const std::string& str1, const std::string& str2);
bool endsWith(const std::string& str1, const std::string& str2);
void split(const std::string& str, const char delimiter, std::vector<std::string>& dest);

inline std::vector<std::string> split(const std::string& str, const char delimiter) {
	std::vector<std::string> result;
	split(str, delimiter, result);
	return result;
}

template<typename T>
inline std::string toString(const T& value) {
	std::ostringstream stream;
	stream << value;
	return stream.str();
}

}

#endif //ROCKET_CMS_STRINGTOOLS_H
