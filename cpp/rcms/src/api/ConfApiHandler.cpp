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

#include "rcms/api/ConfApiHandler.h"

#include <Poco/Util/Application.h>

#include "rcms/tools/ConfigTools.h"

using namespace std;
using namespace Poco::Util;
using namespace Poco::Net;

ConfApiHandler::ConfApiHandler() : AbstractApiHandler("conf") {
}

void ConfApiHandler::handleRequest(ApiConnection& connection) const {
	if (connection.methodName == "getvar") {
		connection.response = ConfigTools::getConfig().getString(connection.args.at("var"));
	}
}
