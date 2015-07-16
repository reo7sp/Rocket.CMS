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

#include "IndexWebHandler.h"

#include <Poco/Path.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include "../tools/FsTools.h"
#include "../tools/NetTools.h"

using namespace Poco;
using namespace Poco::Net;

void IndexWebHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) {
	if (!NetTools::checkAuth(request, response)) {
		return;
	}
	if (request.getURI() == "/") {
		Path filePath(FsTools::getPathFromConfig("fs.rcms.res"), "web/index.html");
		response.send() << FsTools::loadFileToString(filePath);
	} else {
		response.redirect("/", response.HTTP_MOVED_PERMANENTLY);
	}
}
