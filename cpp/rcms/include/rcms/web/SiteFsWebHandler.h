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

#ifndef ROCKETCMS_SITEFSWEBHANDLER_H
#define ROCKETCMS_SITEFSWEBHANDLER_H

#include "rcms/web/AbstractWebHandler.h"

class SiteFsWebHandler : public AbstractWebHandler {

public:
	virtual void handleRequestInternal(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;

private:
	void handleLs(Poco::Net::HTTPServerResponse& response, const Poco::File& file, const Poco::Path& filePath);
	void handleGetFile(Poco::Net::HTTPServerResponse& response, const Poco::File& file, const Poco::Path& filePath);
	void handleCreate(Poco::Net::HTTPServerResponse& response, const Poco::File& file, const Poco::Path& filePath);
	void handleUpload(Poco::Net::HTTPServerResponse& response, const Poco::File& file, const Poco::Path& filePath);
	void handleDelete(Poco::Net::HTTPServerResponse& response, const Poco::File& file, const Poco::Path& filePath, const string& rawFilePath);
	void handleMove(Poco::Net::HTTPServerResponse& response, const Poco::File& fromFile, const Poco::Path& fromFilePath, const Poco::File& file, const Poco::Path& filePath, const string& rawFromFilePath, const string& rawFilePath);
	void handlePublish(Poco::Net::HTTPServerResponse& response, const Poco::File& file, const Poco::Path& filePath, const string& rawFilePath);
	void handleGetMeta(Poco::Net::HTTPServerResponse& response, const Poco::File& file, const Poco::Path& filePath, const std::string& key);
	void handleSetMeta(Poco::Net::HTTPServerResponse& response, const Poco::File& file, const Poco::Path& filePath, const std::string& key, const std::string& value);
	void handleDeleteMeta(Poco::Net::HTTPServerResponse& response, const File& file, const Path& filePath, const std::string& key);
};

#endif //ROCKETCMS_SITEFSWEBHANDLER_H
