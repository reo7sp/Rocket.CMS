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

#include "FilesHandler.h"

#include "AdminServer.h"
#include "../Utils.h"
#include "../TranslationManager.h"
#include "../CacheManager.h"
#include "../ConfigManager.h"
#include "../Log.h"

#include <string>
#include <stdexcept>
#include <boost/algorithm/string/replace.hpp>
#include <boost/filesystem/path.hpp>

using namespace std;
using namespace boost::algorithm;
namespace fs = boost::filesystem;

FilesHandler& FilesHandler::get() {
	static FilesHandler instance;
	return instance;
}

void FilesHandler::displayEdit(mg_connection* connection) const {
	string file;
	try {
		file = Utils::parseUrlQuery(connection->query_string).at("file");
		Utils::urlDecode(file);
	} catch (out_of_range& e) {
	}
	function<void(mg_connection*, string&)> action = [this, &file](mg_connection* connection, string& result) {
		replace_all(result, "%FILEDATA%", "<img src=\"/site/" + _name + "/" + file + "\">");
		replace_all(result, "%FILE%", file);
	};
	AdminServer::handleRequest(connection, _name + "-edit-" + file, _name, "res/admin/" + _name + "-edit.html", action);
}

void FilesHandler::displaySave(mg_connection* connection) const {
	function<void(mg_connection*, string&)> action = [this](mg_connection* connection, string& result) {
		try {
			string file = Utils::parseUrlQuery(string(connection->query_string)).at("file");
			Utils::urlDecode(file);

			fs::path filePath = fs::path(ConfigManager::get().getSitePath() / _name / file);
			bool success = Utils::saveFile(filePath, Utils::loadMultipartData(connection));
			result = TranslationManager::get().getString(success ? "saveok" : "saveerror");

			CacheManager::get().removeString(_name + "-list");
		} catch (out_of_range& e) {
			result = TranslationManager::get().getString("error");
		}
	};
	AdminServer::handleRequest(connection, _name + "-save", "", "", action);
}
