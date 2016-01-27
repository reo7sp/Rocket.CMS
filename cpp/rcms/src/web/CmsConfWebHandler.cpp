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

#include "rcms/web/CmsConfWebHandler.h"

#include <string>

#include <Poco/URI.h>
#include <Poco/String.h>

using namespace std;
using namespace Poco;
using namespace Poco::Net;

void CmsConfWebHandler::handleRequestInternal(HTTPServerRequest& request, HTTPServerResponse& response) {
	static const unsigned long confNameStart = string("/api/cms/conf/").length();
	const string key = replace(URI(request.getURI()).getPath().substr(confNameStart), '/', '.');
	const string result = _core.getConfigManager().getConfig().getString(key);
	response.setContentLength(result.length());
	response.send(result);
}