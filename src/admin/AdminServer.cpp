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
#include "../Log.h"
#include "../Utils.h"
#include "../ConfigManager.h"
#include "../CacheManager.h"
#include "../TranslationManager.h"

#include <string>
#include <sstream>
#include <boost/filesystem/path.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/replace.hpp>

using namespace std;
using namespace boost::algorithm;
namespace fs = boost::filesystem;

AdminServer::AdminServer() {
}

AdminServer::~AdminServer() {
	stop();
}

AdminServer& AdminServer::getInstance() {
	static AdminServer instance;
	return instance;
}

void AdminServer::handleRequest(mg_connection* connection, const std::string& actionName, const std::string& title, const std::string& htmlFile, const std::function<void(mg_connection*, std::string&)>& action) {
	string fileResult = CacheManager::getInstance().get(htmlFile);
	string result = CacheManager::getInstance().get(actionName);
	if (result.empty()) {
		if (fileResult.empty() && !htmlFile.empty()) {
			fileResult = Utils::readFile(fs::path(htmlFile));
			CacheManager::getInstance().set(htmlFile, fileResult);
		}
		result = fileResult;

		TranslationManager::getInstance().translate(result);
		replace_all(result, "%TITLE%", TranslationManager::getInstance().get(title));
		replace_all(result, "%SITETITLE%", ConfigManager::getInstance().getTitle());

		action(connection, result);

		if (!htmlFile.empty()) {
			CacheManager::getInstance().set(actionName, result);
		}
	}
	mg_printf_data(connection, "%s", result.c_str());
}

void AdminServer::start() {
	if (_isRunning) return;
	_isRunning = true;

	Log::info("Starting server at port " + ConfigManager::getInstance().getPort());

	_server = mg_create_server(nullptr, &AdminServer::handleEvent);
	mg_set_option(_server, "listening_port", ConfigManager::getInstance().getPort().c_str());
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
		if (equals(connection->uri, "/")) {
			mg_send_status(connection, 301);
			mg_send_header(connection, "Location", "/pages-list");
			mg_printf_data(connection, "%s", "");
			return MG_TRUE;
		} else if (starts_with(connection->uri, "/pages-list")) {
			PagesHandler::displayPagesList(connection);
			return MG_TRUE;
		} else if (starts_with(connection->uri, "/pages-edit")) {
			PagesHandler::displayPagesEdit(connection);
			return MG_TRUE;
		} else if (starts_with(connection->uri, "/pages-save")) {
			PagesHandler::displayPagesSave(connection);
			return MG_TRUE;
		} else if (starts_with(connection->uri, "/pages-delete")) {
			PagesHandler::displayPagesDelete(connection);
			return MG_TRUE;
		} else if (starts_with(connection->uri, "/template-list")) {
			TemplateHandler::displayTemplateList(connection);
			return MG_TRUE;
		} else if (starts_with(connection->uri, "/template-edit")) {
			TemplateHandler::displayTemplateEdit(connection);
			return MG_TRUE;
		} else if (starts_with(connection->uri, "/template-save")) {
			TemplateHandler::displayTemplateSave(connection);
			return MG_TRUE;
		} else if (starts_with(connection->uri, "/template-delete")) {
			TemplateHandler::displayTemplateDelete(connection);
			return MG_TRUE;
		} else {
			return MG_FALSE;
		}
	} else {
		return MG_FALSE;
	}
}
