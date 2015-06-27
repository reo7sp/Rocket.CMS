/*
 * Copyright 2015 Reo_SP
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ApiManager.h"

#include "../PluginManager.h"

void ApiManager::invokeApiCall(ApiConnection& connection) {
    for (auto iter = ApiManager::_apiHandlers.cbegin(), end = ApiManager::_apiHandlers.cend(); iter != end; ++iter) {
        if ((*iter)->handlerName == connection.handlerName) {
            (*iter)->handleRequest(connection);
            return;
        }
    }
    PluginManager::onApi(connection);
}
