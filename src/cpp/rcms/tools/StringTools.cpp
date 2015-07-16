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

#include "StringTools.h"

#include <sstream>

namespace StringTools {

bool startsWith(const std::string& str1, const std::string& str2) {
	std::string::const_iterator it1(str1.begin());
	std::string::const_iterator end1(str1.end());
	std::string::const_iterator it2(str2.begin());
	std::string::const_iterator end2(str2.end());
	while (it1 != end1 && it2 != end2) {
		if (*it1 != *it2) {
			return false;
		}
		++it1;
		++it2;
	}
	return true;
}

bool endsWith(const std::string& str1, const std::string& str2) {
	std::string::const_iterator it1(str1.end());
	std::string::const_iterator start1(str1.begin());
	std::string::const_iterator it2(str2.begin());
	std::string::const_iterator end2(str2.end());
	while (it1 != start1 && it2 != end2) {
		if (*it1 != *it2) {
			return false;
		}
		--it1;
		++it2;
	}
	return true;
}

void split(const std::string& str, const char delimiter, std::vector<std::string>& dest) {
	std::istringstream stream(str);
	std::string s;
	while (getline(stream, s, delimiter)) {
		dest.push_back(s);
	}
}

}
