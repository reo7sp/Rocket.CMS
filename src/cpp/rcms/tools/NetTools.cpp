/*
 * Copyright 2015 Oleg Morozenkov,
 *		   2015 Alexander Rizaev
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

#include "NetTools.h"

#include <Poco/SHA1Engine.h>
#include <Poco/Util/Application.h>
#include <Poco/Net/HTTPBasicCredentials.h>

namespace NetTools {

bool checkAuth(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
	if (!Poco::Util::Application::instance().config().getBool("web.auth.enabled")) {
		return true;
	}
	if (request.hasCredentials()) {
		Poco::Net::HTTPBasicCredentials credentials(request);
		if (credentials.getUsername() == Poco::Util::Application::instance().config().getString("web.auth.user")) {
			Poco::SHA1Engine sha1Engine;
			sha1Engine.update(credentials.getPassword());
			std::string givenPass = Poco::DigestEngine::digestToHex(sha1Engine.digest());
			std::string pass = Poco::Util::Application::instance().config().getString("web.auth.passhash");
			return givenPass == pass;
		}
	} else {
		response.requireAuthentication("Rocket.CMS");
		response.send();
	}
	return false;
}

}
