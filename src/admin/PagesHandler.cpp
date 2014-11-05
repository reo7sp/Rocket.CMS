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

#include "PagesHandler.h"

#include "../Log.h"
#include "../ConfigManager.h"
#include "../CacheManager.h"
#include "../TranslationManager.h"

#include <string>
#include <sstream>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string/replace.hpp>

using namespace std;
using namespace boost::algorithm;
namespace fs = boost::filesystem;

void PagesHandler::handleRequest(mg_connection* connection) {
	string result = CacheManager::getInstance().get("pageslist");
	if (result.empty()) {
		fs::path file("res/admin/pages.html");
		if (fs::exists(file)) {
			fs::ifstream input(file);
			stringstream buffer;
			string line;
			while (getline(input, line)) {
				buffer << line;
			}
			input.close();

			result = buffer.str();
			TranslationManager::getInstance().translate(result);
			replace_all(result, "%TITLE%", TranslationManager::getInstance().get("pages"));
			replace_all(result, "%SITETITLE%", ConfigManager::getInstance().getTitle());

			string pageslist;
			fs::path dir = ConfigManager::getInstance().getSitePath() / "pages";
			for (auto& item : fs::directory_iterator(dir)) {
				string name = replace_all_copy(item.path().string(), dir.string(), "");
				pageslist += "<tr><td><a href=\"/pages-edit?file=" + name + "\">" + name + "</a></td></tr>";
			}
			replace_all(result, "%PAGESLIST%", pageslist);

			CacheManager::getInstance().set("pageslist", result);
		} else {
			Log::error("res/admin/pages.html doesn't exist");
		}
	}
	mg_printf_data(connection, "%s", result.c_str());
}
