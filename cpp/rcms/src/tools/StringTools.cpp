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

#include "rcms/tools/StringTools.h"

#include <sstream>

using namespace std;

namespace StringTools {

bool startsWith(const string& str1, const string& str2) {
	if (str1.length() < str2.length()) {
		return false;
	}
	string::const_iterator it1(str1.begin());
	string::const_iterator end1(str1.end());
	string::const_iterator it2(str2.begin());
	string::const_iterator end2(str2.end());
	while (it1 != end1 && it2 != end2) {
		if (*it1 != *it2) {
			return false;
		}
		++it1;
		++it2;
	}
	return true;
}

bool endsWith(const string& str1, const string& str2) {
	if (str1.length() < str2.length()) {
		return false;
	}
	string::const_iterator it1(str1.end());
	string::const_iterator begin1(str1.begin());
	string::const_iterator it2(str2.end());
	string::const_iterator begin2(str2.begin());
	while (it1 != begin1 && it2 != begin2) {
		if (*it1 != *it2) {
			return false;
		}
		--it1;
		--it2;
	}
	return true;
}

vector<string> split(const string& str, char delimiter) {
	vector<string> result;
	istringstream stream(str);
	string s;
	while (getline(stream, s, delimiter)) {
		result.push_back(s);
	}
	return result;
}

}
