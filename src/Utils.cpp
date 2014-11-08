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

#include "Utils.h"

#include "Log.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <mongoose/mongoose.h>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/replace.hpp>

using namespace std;
using namespace boost::algorithm;
namespace fs = boost::filesystem;

string Utils::readFile(const fs::path& file) {
	if (fs::exists(file)) {
		fs::ifstream input(file);
		stringstream buffer;
		string line;
		while (getline(input, line)) {
			buffer << line << "\n";
		}
		input.close();
		return buffer.str();
	} else {
		Log::warn(file.string() + " doesn't exist");
		return "";
	}
}

bool Utils::saveFile(const fs::path& file, const string& text) {
	if (fs::exists(file)) {
		fs::ofstream output(file, ios_base::trunc);
		output << text;
		output.close();
		return true;
	} else {
		Log::warn(file.string() + " doesn't exist");
		return false;
	}
}

void Utils::urlEncode(string& url) {
	size_t len = url.length();

	char* dst = new char[len * 3 + 1];
	copy(url.begin(), url.end(), dst);
	fill(dst + len, dst + len * 3, ' ');
	dst[len * 3] = '\0';

	mg_url_encode(url.c_str(), len, dst, len * 3);
	url = trim_copy(string(dst));

	delete[] dst;
}

void Utils::urlDecode(string& url) {
	size_t len = url.length();

	char* dst = new char[len + 1];
	copy(url.begin(), url.end(), dst);
	dst[len] = '\0';

	mg_url_decode(url.c_str(), len, dst, len, 0);
	url = trim_copy(string(dst));

	delete[] dst;
}

void Utils::htmlEncode(string& text) {
	replace_all(text, "&", "&amp;");
	replace_all(text, " ", "&nbsp;");
	replace_all(text, "<", "&lt;");
	replace_all(text, ">", "&gt;");
	replace_all(text, "\n", "<br>");
}

void Utils::htmlDecode(string& text) {
	replace_all(text, "<br>", "\n");
	boost::regex_replace(text, htmlTagsRegex, "");
	replace_all(text, "&nbsp;", " ");
	replace_all(text, "&lt;", "<");
	replace_all(text, "&gt;", ">");
	replace_all(text, "&amp;", "&");
}

string Utils::postDataParse(const string& httpContent) {
	stringstream in(httpContent);
	stringstream out;
	string line;
	bool headersEnded = false;
	while (getline(in, line)) {
		if (headersEnded) {
			out << line << "\n";
		}
		if (line == "\r") {
			headersEnded = true;
		}
	}
	string result = headersEnded ? out.str() : httpContent;
	result = result.substr(0, result.rfind("###END###"));
	return replace_all_copy(result, "\r", "");
}

map<string, string> Utils::parseUrlQuery(const string& query) {
	map<string, string> result;
	vector<string> args;
	split(args, query, is_any_of("&"));
	for (auto& item : args) {
		vector<string> parts;
		split(parts, item, is_any_of("="));
		result[parts[0]] = parts.size() == 1 ? "" : parts[1];
	}
	return result;
}
