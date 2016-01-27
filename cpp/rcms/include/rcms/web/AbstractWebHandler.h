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

#ifndef ROCKETCMS_ABSTRACTWEBHANDLER_H
#define ROCKETCMS_ABSTRACTWEBHANDLER_H

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

class AbstractWebHandler : public Poco::Net::HTTPRequestHandler {

public:
	AbstractWebHandler(Core core) : _core(Core) { }

	virtual void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;

protected:
	virtual void handleRequestInternal(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) = 0;
	void sendError(Poco::Net::HTTPServerResponse& response, Poco::Net::HTTPResponse::HTTPStatus errorCode = Poco::Net::HTTPResponse::HTTPStatus::HTTP_INTERNAL_SERVER_ERROR, const std::string& shortDescription = "") const;

	Core _core;

private:
	bool checkAuth(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) const;
};

#endif //ROCKETCMS_ABSTRACTWEBHANDLER_H
