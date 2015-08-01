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

#include "rcms/pluginapi/rcms_Core.h"

#include <string>

#include <Poco/SharedPtr.h>

#include "rcms/CacheManager.h"
#include "rcms/api/ApiConnection.h"
#include "rcms/api/ApiManager.h"
#include "rcms/tools/FsTools.h"

using namespace std;
using namespace Poco;

void rcms_Core_invokeApiCall(const rcms_ApiConnection* connection) {
	ApiConnection cppConnection;
	cppConnection.handlerName = string(connection->handler);
	cppConnection.methodName = string(connection->method);
	for (size_t i = 0; i < connection->argsCount; ++i) {
		rcms_StrPair arg = connection->args[i];
		cppConnection.args[string(arg.first)] = string(arg.second);
	}
	if (connection->postData != NULL) {
		cppConnection.postData = string(connection->postData);
	}
	cppConnection.response = string(connection->response);
	cppConnection.responseMimeType = string(connection->responseMimeType);
	ApiManager::getInstance().invokeApiCall(cppConnection);
}

void rcms_Core_saveToCache(const char* name, const char* value) {
	CacheManager::getInstance().getGeneralCache().add(string(name), SharedPtr<string>(new string(value)));
}

const char* rcms_Core_getFromCache(const char* name) {
	SharedPtr<string> result = CacheManager::getInstance().getGeneralCache().get(string(name));
	if (result.isNull()) {
		return NULL;
	} else {
		return result->c_str();
	}
}

void rcms_Core_invalidateCacheItem(const char* name) {
	CacheManager::getInstance().getGeneralCache().remove(string(name));
}

const char* rcms_Core_loadFile(const char* name) {
	return FsTools::loadFileToString(string(name)).c_str();
}
