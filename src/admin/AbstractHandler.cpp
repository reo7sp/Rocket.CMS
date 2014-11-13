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

#include "AbstractHandler.h"

#include "AdminServer.h"
#include "../Utils.h"
#include "../ConfigManager.h"
#include "../CacheManager.h"
#include "../TranslationManager.h"
#include "../PageCompiler.h"

#include <algorithm>
#include <functional>
#include <stdexcept>
#include <mongoose/mongoose.h>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/replace.hpp>

using namespace std;
using namespace boost::algorithm;
namespace fs = boost::filesystem;

bool AbstractHandler::tryDisplay(mg_connection* connection) const {
	if (starts_with(connection->uri, "/" + _name + "-list")) {
		displayList(connection);
		return true;
	} else if (starts_with(connection->uri, "/" + _name + "-edit")) {
		displayEdit(connection);
		return true;
	} else if (starts_with(connection->uri, "/" + _name + "-save")) {
		displaySave(connection);
		return true;
	} else if (starts_with(connection->uri, "/" + _name + "-delete")) {
		displayDelete(connection);
		return true;
	}
	return false;
}

void AbstractHandler::displayList(mg_connection* connection) const {
	function<void(mg_connection*, string&)> action = [this](mg_connection* connection, string& result) {
		string fileList;
		fs::path dir = ConfigManager::get().getSitePath() / _name;
		fs::recursive_directory_iterator endIter;
		for (fs::recursive_directory_iterator iter(dir); iter != endIter; ++iter) {
			string file = replace_all_copy(iter->path().string(), dir.string() + "/", "");
			string fileName = iter->path().filename().string();

			size_t slashCount = count(file.begin(), file.end(), '/');
			fileList += "<tr><td style=\"padding-left:" + to_string(slashCount * 1.25) + "em\">";

			if (fs::is_directory(iter->path())) {
				fileList += "▼ " + file;
			} else {
				string fileUrl = file;
				Utils::urlEncode(fileUrl);
				fileList += "<a href=\"/" + _name + "-edit?file=" + fileUrl + "\">" + fileName + "</a>";
				fileList += "<a href=\"/" + _name + "-delete?file=" + fileUrl + "\" class=\"delete-btn\">" + TranslationManager::get().getString("delete") + "</a>";
			}

			fileList += "</td></tr>";
		}
		replace_all(result, "%FILELIST%", fileList);
	};
	AdminServer::handleRequest(connection, _name + "-list", _name, "res/admin/" + _name + "-list.html", action);
}

void AbstractHandler::displayEdit(mg_connection* connection) const {
	string file;
	try {
		file = Utils::parseUrlQuery(connection->query_string).at("file");
		Utils::urlDecode(file);
	} catch (out_of_range& e) {
	}
	function<void(mg_connection*, string&)> action = [this, &file](mg_connection* connection, string& result) {
		string text = Utils::readFile(fs::path(ConfigManager::get().getSitePath() / _name / file));
		Utils::htmlEncode(text);
		replace_all(result, "%FILEDATA%", text);
		replace_all(result, "%FILE%", file);
	};
	AdminServer::handleRequest(connection, _name + "-edit-" + file, _name, "res/admin/" + _name + "-edit.html", action);
}

void AbstractHandler::displaySave(mg_connection* connection) const {
	function<void(mg_connection*, string&)> action = [this](mg_connection* connection, string& result) {
		try {
			string file = Utils::parseUrlQuery(string(connection->query_string)).at("file");
			Utils::urlDecode(file);
			string text = Utils::postDataParse(connection);
			Utils::htmlDecode(text);

			fs::path filePath = fs::path(ConfigManager::get().getSitePath() / _name / file);
			bool success = Utils::saveFile(filePath, text);
			result = TranslationManager::get().getString(success ? "saveok" : "saveerror");

			CacheManager::get().removeString(_name + "-list");
			CacheManager::get().removeString(_name + "-edit-" + file);
		} catch (out_of_range& e) {
			result = TranslationManager::get().getString("error");
		}
	};
	AdminServer::handleRequest(connection, _name + "-save", "", "", action);
}

void AbstractHandler::displayDelete(mg_connection* connection) const {
	function<void(mg_connection*, string&)> action = [this](mg_connection* connection, string& result) {
		try {
			string file = Utils::parseUrlQuery(string(connection->query_string)).at("file");
			Utils::urlDecode(file);

			fs::remove(ConfigManager::get().getSitePath() / _name / file);
			fs::remove(ConfigManager::get().getSitePath() / "public" / file);

			CacheManager::get().removeString(_name + "-list");
			CacheManager::get().removeString(_name + "-edit-" + file);
		} catch (out_of_range& e) {
		}
		mg_send_status(connection, 301);
		mg_send_header(connection, "Location", ("/" + _name + "-list").c_str());
	};
	AdminServer::handleRequest(connection, _name + "-delete", _name, "res/admin/" + _name + "-delete.html", action);
}
