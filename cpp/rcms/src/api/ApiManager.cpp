/*
 * Copyright 2015 Oleg Morozenkov
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

#include "rcms/api/ApiManager.h"

#include "rcms/PluginManager.h"

using namespace std;

ApiManager& ApiManager::getInstance() {
	static ApiManager result;
	return result;
}

void ApiManager::invokeApiCall(ApiConnection& connection) {
	try {
		for (auto iter = _apiHandlers.cbegin(), end = _apiHandlers.cend(); iter != end; ++iter) {
			if ((*iter)->handlerName == connection.handlerName) {
				(*iter)->handleRequest(connection);
				return;
			}
		}
		if (!PluginManager::getInstance().onApi(connection) && connection.responseCode < 400) {
			connection.responseCode = 500;
		}
	} catch (out_of_range& e) {
		if (connection.responseCode < 400) {
			connection.responseCode = 400;
			connection.response = e.what();
			connection.responseMimeType = "text/plain";
		}
	} catch (exception& e) {
		if (connection.responseCode < 400) {
			connection.responseCode = 500;
			connection.response = e.what();
			connection.responseMimeType = "text/plain";
		}
	}
}
