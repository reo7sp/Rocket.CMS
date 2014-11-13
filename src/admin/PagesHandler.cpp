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

#include "../Utils.h"
#include "../ConfigManager.h"
#include "../PageCompiler.h"

#include <string>
#include <boost/filesystem/path.hpp>

using namespace std;
namespace fs = boost::filesystem;

PagesHandler& PagesHandler::get() {
	static PagesHandler instance;
	return instance;
}

void PagesHandler::displaySave(mg_connection* connection) const {
	AbstractHandler::displaySave(connection);

	string file = Utils::parseUrlQuery(string(connection->query_string)).at("file");
	Utils::urlDecode(file);
	fs::path filePath = fs::path(ConfigManager::get().getSitePath() / _name / file);
	PageCompiler::get().compile(filePath);
}
