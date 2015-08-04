/*
 * Copyright 2015 Oleg Morozenkov
 * Copyright 2015 Alexander Rizaev
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

#include "rcms/tools/NetTools.h"

#include <Poco/SHA1Engine.h>
#include <Poco/Util/Application.h>
#include <Poco/Net/HTTPBasicCredentials.h>

#include "rcms/tools/ConfigTools.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;

namespace NetTools {

bool checkAuth(HTTPServerRequest& request, HTTPServerResponse& response) {
	if (!ConfigTools::getConfig().getBool("web.auth.enabled")) {
		return true;
	}
	if (request.hasCredentials()) {
		HTTPBasicCredentials credentials(request);
		if (credentials.getUsername() == ConfigTools::getConfig().getString("web.auth.user")) {
			SHA1Engine sha1Engine;
			sha1Engine.update(credentials.getPassword());
			string givenPass = DigestEngine::digestToHex(sha1Engine.digest());
			string pass = ConfigTools::getConfig().getString("web.auth.passhash");
			return givenPass == pass;
		}
	} else {
		response.requireAuthentication("Rocket.CMS");
		response.setContentLength(0);
		response.send();
	}
	return false;
}

void sendError(HTTPServerResponse& response, HTTPResponse::HTTPStatus errorCode, const string& shortDescription) {
	response.setStatus(errorCode);
	response.setContentType("text/html");
	response.setChunkedTransferEncoding(true);
	response.send() << ""
		"<!DOCTYPE html>\n"
		"<html>\n"
		"	<head>\n"
		"		<meta charset=\"utf-8\">\n"
		"		<title>" << errorCode << "</title>\n"
		"	</head>\n"
		"	<body>\n"
		"		<h1>" << shortDescription << "</h1>\n"
		"	</body>\n"
		"</html>\n";
}

}
