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

#include "rcms/web/WebguiWebHandler.h"

#include <fstream>

#include <Poco/URI.h>
#include <Poco/Path.h>

using namespace std;
using namespace Poco;
using namespace Poco::Net;

void WebguiWebHandler::handleRequestInternal(HTTPServerRequest& request, HTTPServerResponse& response) {
	URI uri = URI(request.getURI());
	Path filePath(_core.getConfigManager().extractPath("fs.cms.root"), uri.getPath());
	ifstream stream(filePath.toString(), ios::ate);

	response.setContentType("text/html");
	response.setContentLength(stream.tellg());
	stream.seekg(0, stream.beg);
	response.send() << stream;
}
