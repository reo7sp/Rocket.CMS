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

#include "rcms/web/IndexWebHandler.h"

#include <exception>

#include <Poco/Path.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include "rcms/tools/NetTools.h"
#include "rcms/tools/ConfigTools.h"
#include "rcms/tools/FsTools.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;

void IndexWebHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) {
	try {
		if (!NetTools::checkAuth(request, response)) {
			return;
		}
		response.setChunkedTransferEncoding(true);
		if (request.getURI() == "/") {
			Path filePath(ConfigTools::getPathFromConfig("fs.cms.root"), "webgui/index.html");
			response.setContentType("text/html");
			response.send() << FsTools::loadFileToString(filePath);
		} else {
			response.redirect("/", response.HTTP_MOVED_PERMANENTLY);
		}
	} catch (exception& e) {
		NetTools::sendError(response, HTTPResponse::HTTPStatus::HTTP_INTERNAL_SERVER_ERROR, e.what());
	}
}
