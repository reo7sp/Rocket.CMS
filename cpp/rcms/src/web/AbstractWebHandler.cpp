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

#include "rcms/web/AbstractWebHandler.h"

#include <exception>

#include <Poco/SHA1Engine.h>
#include <Poco/Net/HTTPBasicCredentials.h>

void NetTools::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
	try {
		if (!checkAuth(request, response)) {
			return;
		}
		handleRequestInternal(request, response);
	} catch (std::exception& e) {
		sendError(response, Poco::Net::HTTPResponse::HTTPStatus::HTTP_INTERNAL_SERVER_ERROR, e.what());
	}
}

bool NetTools::checkAuth(HTTPServerRequest& request, HTTPServerResponse& response) {
	const auto config = _core.getConfigManager().getConfig();
	if (!config.getBool("web.auth.enabled")) {
		return true;
	}
	if (request.hasCredentials()) {
		const HTTPBasicCredentials credentials(request);
		if (credentials.getUsername() == config.getString("web.auth.user")) {
			const SHA1Engine sha1Engine;
			sha1Engine.update(credentials.getPassword());
			const string givenPass = DigestEngine::digestToHex(sha1Engine.digest());
			const string pass = config.getString("web.auth.passhash");
			return givenPass == pass;
		}
	} else {
		response.requireAuthentication("Rocket.CMS");
		response.setContentLength(0);
		response.send();
	}
	return false;
}

void NetTools::sendError(HTTPServerResponse& response, HTTPResponse::HTTPStatus errorCode, const string& description) {
	if (description.empty()) {
		response.setStatus(errorCode);
	} else {
		response.setStatusAndReason(errorCode, description);
	}
	response.setContentType("text/plain");
	response.setContentLength(0);
	response.send();
}
