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

#include "ApiWebHandler.h"

#include <string>
#include <map>

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPBasicCredentials.h>

#include "../tools/StringTools.h"
#include "../tools/NetTools.h"
#include "api/FsApiHandler.h"
#include "api/ConfApiHandler.h"
#include "api/WebguiApiHandler.h"

using namespace std;
using namespace Poco::Net;

ApiWebHandler::ApiWebHandler() : HTTPRequestHandler(), _apiHandlers({ new FsApiHandler(), new ConfApiHandler(), new WebguiApiHandler() }) {
}

ApiWebHandler::~ApiWebHandler() {
    for (auto item : _apiHandlers) {
        delete item;
    }
}

void ApiWebHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) {
    if (!NetTools::checkAuth(request, response)) {
        return;
    }

    static const size_t handlerNamePos = 5; // the length of string "/api/" is 5
    const string& uri = request.getURI();

    string handlerName = uri.substr(handlerNamePos, uri.find('/', handlerNamePos) - handlerNamePos);
    size_t methodNamePos = handlerNamePos + handlerName.length() + 1;
    string methodName = uri.substr(methodNamePos, uri.find('?', methodNamePos) - methodNamePos);
    size_t argsPos = methodNamePos + methodName.length() + 1;

    map<string, string> args;
    if (argsPos < uri.length()) {
        string argsString = uri.substr(argsPos);
        vector<string> rawArgsArray = StringTools::split(argsString, '?');
        for (string item : rawArgsArray) {
            vector<string> rawArgParts = StringTools::split(item, '=');
            args[rawArgParts.at(0)] = rawArgParts.at(1);
        }
    }

    for (AbstractApiHandler* item : _apiHandlers) {
        if (item->isHandlerNameEquals(handlerName)) {
            item->handleRequest(methodName, args, request, response);
            break;
        }
    }
}
