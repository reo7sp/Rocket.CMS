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

#include "ConfApiHandler.h"

#include <Poco/Util/Application.h>

using namespace std;
using namespace Poco::Util;
using namespace Poco::Net;

bool ConfApiHandler::isHandlerNameEquals(const string& handlerName) {
    return handlerName == "conf";
}

void ConfApiHandler::handleRequest(const string& methodName, const map<string, string>& args,
                                   HTTPServerRequest& request, HTTPServerResponse& response) {
    if (methodName == "getvar") {
        response.send() << Application::instance().config().getString(args.at("var"));
    }
}
