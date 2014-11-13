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

#include "TemplateHandler.h"

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
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/split.hpp>

using namespace std;
using namespace boost::algorithm;
namespace fs = boost::filesystem;

TemplateHandler::TemplateHandler() {
}

TemplateHandler::~TemplateHandler() {
}

TemplateHandler& TemplateHandler::get() {
	static TemplateHandler instance;
	return instance;
}

bool TemplateHandler::tryDisplay(mg_connection* connection) {
	if (starts_with(connection->uri, "/template-list")) {
		displayTemplateList(connection);
		return true;
	} else if (starts_with(connection->uri, "/template-edit")) {
		displayTemplateEdit(connection);
		return true;
	} else if (starts_with(connection->uri, "/template-save")) {
		displayTemplateSave(connection);
		return true;
	} else if (starts_with(connection->uri, "/template-delete")) {
		displayTemplateDelete(connection);
		return true;
	}
	return false;
}

void TemplateHandler::displayTemplateList(mg_connection* connection) {
	string actionName = "template-list";
	string title = "template";
	string htmlFile = "res/admin/template-list.html";
	function<void(mg_connection*, string&)> action = [](mg_connection* connection, string& result) {
		string templatelist;
		fs::path dir = ConfigManager::get().getSitePath() / "template";
		fs::recursive_directory_iterator endIter;
		for (fs::recursive_directory_iterator iter(dir); iter != endIter; ++iter) {
			string file = replace_all_copy(iter->path().string(), dir.string() + "/", "");
			string fileName = iter->path().filename().string();

			size_t slashCount = count(file.begin(), file.end(), '/');
			templatelist += "<tr><td style=\"padding-left:" + to_string(slashCount * 1.25) + "em\">";

			if (fs::is_directory(iter->path())) {
				templatelist += "▼ " + file;
			} else {
				string fileUrl = file;
				Utils::urlEncode(fileUrl);
				templatelist += "<a href=\"/template-edit?file=" + fileUrl + "\">" + fileName + "</a>";
				templatelist += "<a href=\"/template-delete?file=" + fileUrl + "\" class=\"delete-btn\">" + TranslationManager::get().getString("delete") + "</a>";
			}

			templatelist += "</td></tr>";
		}
		replace_all(result, "%TEMPLATELIST%", templatelist);
	};
	AdminServer::handleRequest(connection, actionName, title, htmlFile, action);
}

void TemplateHandler::displayTemplateEdit(mg_connection* connection) {
	string file;
	try {
		file = Utils::parseUrlQuery(connection->query_string).at("file");
		Utils::urlDecode(file);
	} catch (out_of_range& e) {
	}

	string actionName = "template-edit-" + file;
	string title = "template";
	string htmlFile = "res/admin/template-edit.html";
	function<void(mg_connection*, string&)> action = [&file](mg_connection* connection, string& result) {
		string text = Utils::readFile(fs::path(ConfigManager::get().getSitePath() / "template" / file));
		Utils::htmlEncode(text);
		replace_all(result, "%TEMPLATEDATA%", text);
		replace_all(result, "%FILE%", file);
	};
	AdminServer::handleRequest(connection, actionName, title, htmlFile, action);
}

void TemplateHandler::displayTemplateSave(mg_connection* connection) {
	string actionName = "template-save";
	string title = "";
	string htmlFile = "";
	function<void(mg_connection*, string&)> action = [](mg_connection* connection, string& result) {
		try {
			string file = Utils::parseUrlQuery(string(connection->query_string)).at("file");
			Utils::urlDecode(file);
			string text = Utils::postDataParse(connection);
			Utils::htmlDecode(text);

			fs::path filePath = fs::path(ConfigManager::get().getSitePath() / "template" / file);
			bool success = Utils::saveFile(filePath, text);
			result = TranslationManager::get().getString(success ? "saveok" : "saveerror");

			CacheManager::get().removeString("template-list");
			CacheManager::get().removeString("template-edit-" + file);
		} catch (out_of_range& e) {
			result = TranslationManager::get().getString("error");
		}
	};
	AdminServer::handleRequest(connection, actionName, title, htmlFile, action);
}

void TemplateHandler::displayTemplateDelete(mg_connection* connection) {
	string actionName = "template-delete";
	string title = "";
	string htmlFile = "";
	function<void(mg_connection*, string&)> action = [](mg_connection* connection, string& result) {
		try {
			string file = Utils::parseUrlQuery(string(connection->query_string)).at("file");
			Utils::urlDecode(file);

			fs::remove(ConfigManager::get().getSitePath() / "template" / file);

			CacheManager::get().removeString("template-list");
			CacheManager::get().removeString("template-edit-" + file);
		} catch (out_of_range& e) {
		}
		mg_send_status(connection, 301);
		mg_send_header(connection, "Location", "/template-list");
	};
	AdminServer::handleRequest(connection, actionName, title, htmlFile, action);
}
