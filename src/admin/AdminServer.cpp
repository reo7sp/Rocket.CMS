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

#include "AdminServer.h"

#include "PagesHandler.h"
#include "TemplateHandler.h"
#include "FilesHandler.h"
#include "../Log.h"
#include "../Utils.h"
#include "../ConfigManager.h"
#include "../CacheManager.h"
#include "../TranslationManager.h"

#include <string>
#include <sstream>
#include <stdexcept>
#include <boost/filesystem/path.hpp>
#include <boost/algorithm/string/replace.hpp>

using namespace std;
using namespace boost::algorithm;
namespace fs = boost::filesystem;

AdminServer::AdminServer() {
}

AdminServer::~AdminServer() {
	stop();
}

AdminServer& AdminServer::get() {
	static AdminServer instance;
	return instance;
}

void AdminServer::handleRequest(mg_connection* connection, const std::string& actionName, const std::string& title, const std::string& htmlFile, const std::function<void(mg_connection*, std::string&)>& action) {
	string fileResult = CacheManager::get().getString(htmlFile);
	string result = CacheManager::get().getString(actionName);
	if (result.empty()) {
		if (fileResult.empty() && !htmlFile.empty()) {
			fileResult = Utils::readFile(fs::path(htmlFile));
			CacheManager::get().setString(htmlFile, fileResult);
		}
		result = fileResult;

		TranslationManager::get().translate(result);
		replace_all(result, "%TITLE%", TranslationManager::get().getString(title));
		replace_all(result, "%SITETITLE%", ConfigManager::get().getTitle());

		action(connection, result);

		if (!htmlFile.empty()) {
			CacheManager::get().setString(actionName, result);
		}
	}
	mg_printf_data(connection, "%s", result.c_str());
}

void AdminServer::start() {
	if (_isRunning) return;
	_isRunning = true;

	Log::info("Starting server at port " + ConfigManager::get().getPort());

	_server = mg_create_server(nullptr, &AdminServer::handleEvent);
	mg_set_option(_server, "listening_port", ConfigManager::get().getPort().c_str());
	mg_set_option(_server, "document_root", fs::path("res/admin").c_str());
	while (_isRunning) {
		mg_poll_server(_server, 1000);
	}
	mg_destroy_server(&_server);
	_server = nullptr;

	Log::info("Server has been stopped");
}

void AdminServer::stop() {
	_isRunning = false;
}

int AdminServer::handleEvent(mg_connection* connection, mg_event event) {
	if (event == MG_AUTH) {
		return MG_TRUE;
	} else if (event == MG_REQUEST) {
		try {
			if (string(connection->uri) == "/") {
				mg_send_status(connection, 301);
				mg_send_header(connection, "Location", "/pages-list");
				mg_printf_data(connection, "%s", "");
				return MG_TRUE;
			} else {
				if (PagesHandler::get().tryDisplay(connection)) return MG_TRUE;
				if (TemplateHandler::get().tryDisplay(connection)) return MG_TRUE;
				if (FilesHandler::get().tryDisplay(connection)) return MG_TRUE;
				return MG_FALSE;
			}
		} catch (exception& e) {
			mg_send_status(connection, 500);
			mg_printf_data(connection, "%s\r\n\r\n%s", "500 Internal Server Error", e.what());
			return MG_TRUE;
		}
	} else {
		return MG_FALSE;
	}
}
