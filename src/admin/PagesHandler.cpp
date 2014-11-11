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

#include "AdminServer.h"
#include "../Log.h"
#include "../Utils.h"
#include "../ConfigManager.h"
#include "../CacheManager.h"
#include "../TranslationManager.h"
#include "../PageCompiler.h"

#include <string>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <mongoose/mongoose.h>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/split.hpp>

using namespace std;
using namespace boost::algorithm;
namespace fs = boost::filesystem;

void PagesHandler::displayPagesList(mg_connection* connection) {
	string actionName = "pages-list";
	string title = "pages";
	string htmlFile = "res/admin/pages-list.html";
	function<void(mg_connection*, string&)> action = [](mg_connection* connection, string& result) {
		string pageslist;
		fs::path dir = ConfigManager::getInstance().getSitePath() / "pages";
		fs::recursive_directory_iterator endIter;
		for (fs::recursive_directory_iterator iter(dir); iter != endIter; ++iter) {
			string file = replace_all_copy(iter->path().string(), dir.string(), "");

			size_t slashCount = count(file.begin(), file.end(), '/');
			pageslist += "<tr><td style=\"padding-left:" + to_string((slashCount - 1) * 1.25) + "em\">";

			if (fs::is_directory(iter->path())) {
				pageslist += "▼ " + file;
			} else {
				string fileUrl = file;
				Utils::urlEncode(fileUrl);
				pageslist += "<a href=\"/pages-edit?file=" + fileUrl + "\">" + file + "</a>";
				pageslist += "<a href=\"/pages-delete?file=" + fileUrl + "\" class=\"delete-btn\">" + TranslationManager::getInstance().get("delete") + "</a>";
			}

			pageslist += "</td></tr>";
		}
		replace_all(result, "%PAGESLIST%", pageslist);
	};
	AdminServer::handleRequest(connection, actionName, title, htmlFile, action);
}

void PagesHandler::displayPagesEdit(mg_connection* connection) {
	string file;
	try {
		file = Utils::parseUrlQuery(connection->query_string).at("file");
		Utils::urlDecode(file);
	} catch (out_of_range& e) {
	}

	string actionName = "pages-edit-" + file;
	string title = "pages";
	string htmlFile = "res/admin/pages-edit.html";
	function<void(mg_connection*, string&)> action = [&file](mg_connection* connection, string& result) {
		string text = Utils::readFile(fs::path(ConfigManager::getInstance().getSitePath() / "pages" / file));
		Utils::htmlEncode(text);
		replace_all(result, "%PAGEDATA%", text);
		replace_all(result, "%FILE%", file);
	};
	AdminServer::handleRequest(connection, actionName, title, htmlFile, action);
}

void PagesHandler::displayPagesSave(mg_connection* connection) {
	string actionName = "pages-save";
	string title = "";
	string htmlFile = "";
	function<void(mg_connection*, string&)> action = [](mg_connection* connection, string& result) {
		try {
			string file = Utils::parseUrlQuery(string(connection->query_string)).at("file");
			Utils::urlDecode(file);
			string text = Utils::postDataParse(connection);
			Utils::htmlDecode(text);

			fs::path filePath = fs::path(ConfigManager::getInstance().getSitePath() / "pages" / file);
			bool success = Utils::saveFile(filePath, text);
			result = TranslationManager::getInstance().get(success ? "saveok" : "saveerror");
			PageCompiler::getInstance().compile(filePath);

			CacheManager::getInstance().invalidate("pages-list");
			CacheManager::getInstance().invalidate("pages-edit-" + file);
		} catch (out_of_range& e) {
			result = TranslationManager::getInstance().get("error");
		}
	};
	AdminServer::handleRequest(connection, actionName, title, htmlFile, action);
}

void PagesHandler::displayPagesDelete(mg_connection* connection) {
	string actionName = "pages-delete";
	string title = "";
	string htmlFile = "";
	function<void(mg_connection*, string&)> action = [](mg_connection* connection, string& result) {
		try {
			string file = Utils::parseUrlQuery(string(connection->query_string)).at("file");
			Utils::urlDecode(file);

			fs::remove(ConfigManager::getInstance().getSitePath() / "pages" / file);
			fs::remove(ConfigManager::getInstance().getSitePath() / "public" / file);

			CacheManager::getInstance().invalidate("pages-list");
			CacheManager::getInstance().invalidate("pages-edit-" + file);
		} catch (out_of_range& e) {
		}
		mg_send_status(connection, 301);
		mg_send_header(connection, "Location", "/pages-list");
	};
	AdminServer::handleRequest(connection, actionName, title, htmlFile, action);
}
