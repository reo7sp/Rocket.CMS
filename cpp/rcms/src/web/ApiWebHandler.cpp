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

#include "rcms/web/ApiWebHandler.h"

#include <map>

#include <Poco/URI.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPBasicCredentials.h>

#include "rcms/tools/StringTools.h"
#include "rcms/tools/NetTools.h"
#include "rcms/api/ApiConnection.h"
#include "rcms/api/ApiManager.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;

void ApiWebHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) {
	try {
		if (!NetTools::checkAuth(request, response)) {
			return;
		}
		ApiConnection connection;

		const string& uri = request.getURI();
		static const size_t handlerNamePos = 5; // the length of string "/api/" is 5
		connection.handlerName = uri.substr(handlerNamePos, uri.find('/', handlerNamePos) - handlerNamePos);
		size_t methodNamePos = handlerNamePos + connection.handlerName.length() + 1;
		connection.methodName = uri.substr(methodNamePos, uri.find('?', methodNamePos) - methodNamePos);
		size_t argsPos = methodNamePos + connection.methodName.length() + 1;
		string argsString = uri.substr(argsPos);
		vector<string> rawArgsArray = StringTools::split(argsString, '?');
		for (string& item : rawArgsArray) {
			vector<string> rawArgParts = StringTools::split(item, '=');
			string name;
			string value;
			URI::decode(rawArgParts.at(0), name);
			URI::decode(rawArgParts.at(1), value);
			replaceInPlace(value, "../", "");
			connection.args[name] = value;
		}

		if (request.getMethod() == request.HTTP_POST) {
			istream& input = request.stream();
			char* data = new char[request.getContentLength()];
			input.read(data, request.getContentLength());
			connection.postData = string(data, (unsigned long) input.gcount());
		}

		ApiManager::getInstance().invokeApiCall(connection);
		response.setContentType(connection.responseMimeType);
		response.setContentLength(connection.response.length());
		response.setStatus((HTTPResponse::HTTPStatus) connection.responseCode);
		if (connection.responseCode > 399) {
			response.send() << "<h1>Can't process your api call. Error code: "
				<< StringTools::toString(connection.responseCode)
				<< "</h1>"
				<< connection.response;
		} else {
			ostream& output = response.send();
			output.write(connection.response.c_str(), connection.response.length());
		}
	} catch (exception& e) {
		response.setStatus(HTTPResponse::HTTP_REASON_INTERNAL_SERVER_ERROR);
		response.send() << "<h1>Can't handle request. Error code: 500</h1>" << e.what();
	}
}
