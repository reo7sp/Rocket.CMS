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

#include "FsApiHandler.h"

#include <Poco/Util/Application.h>
#include <Poco/File.h>

#include "../FsEvent.h"
#include "../PluginManager.h"
#include "../tools/FsTools.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::Util;


FsApiHandler::FsApiHandler() : AbstractApiHandler("fs") {
}

void FsApiHandler::handleRequest(ApiConnection& connection) const {
    if (connection.methodName == "ls") {
        Path dirPath(FsTools::getPathFromConfig("fs.site.src"), connection.args.at("dir"));
        vector<File> files;
        File(dirPath).list(files);

        string& jsonResponseStr = connection.response;
        jsonResponseStr += '[';
        size_t lastSlashPos = dirPath.toString(Path::PATH_UNIX).rfind('/');
        for (File& item : files) {
            jsonResponseStr += "{name:\"";
            jsonResponseStr += Path(item.path().substr(lastSlashPos + 1)).toString(Path::PATH_UNIX);
            jsonResponseStr += "\",isdir:";
            jsonResponseStr += item.isDirectory() ? '1' : '0';
            jsonResponseStr += "},";
        }
        jsonResponseStr.erase(jsonResponseStr.length() - 2);
        jsonResponseStr += ']';

        connection.responseMimeType = "application/json";
    } else if (connection.methodName == "getfile") {
        Path filePath(FsTools::getPathFromConfig("fs.site.src"), connection.args.at("file"));

        FsTools::loadFileToString(filePath, false);
        connection.responseMimeType = FsTools::getMimeType(filePath);
    } else if (connection.methodName == "getmeta") {
        // TODO
    } else if (connection.methodName == "rm") {
        Path filePath(FsTools::getPathFromConfig("fs.site.src"), connection.args.at("file"));
        Path fileDestPath(FsTools::getPathFromConfig("fs.site.dst"), connection.args.at("file"));

        File(filePath).remove(true);
        File fileDestFile(fileDestPath);
        if (fileDestFile.exists()) {
            fileDestFile.remove(true);
        }

        PluginManager::onFs(FsEvent(FsEvent::Type::RM, filePath));

        connection.response = "1";
    } else if (connection.methodName == "mv") {
        Path fromPath(FsTools::getPathFromConfig("fs.site.src"), connection.args.at("from"));
        Path toPath(FsTools::getPathFromConfig("fs.site.src"), connection.args.at("to"));

        File(fromPath).moveTo(toPath.toString());

        PluginManager::onFs(FsEvent(FsEvent::Type::MV, fromPath, toPath));

        connection.response = "1";
    } else if (connection.methodName == "create") {
        // TODO
    } else if (connection.methodName == "upload") {
        // TODO
    } else if (connection.methodName == "edit") {
        // TODO
    } else if (connection.methodName == "setmeta") {
        // TODO
    } else if (connection.methodName == "publish") {
        Path filePath(FsTools::getPathFromConfig("fs.site.src"), connection.args.at("file"));
        Path fileDestPath(FsTools::getPathFromConfig("fs.site.dst"), connection.args.at("file"));

        File(filePath).moveTo(fileDestPath.toString());

        PluginManager::onFs(FsEvent(FsEvent::Type::PUBLISH, filePath));

        connection.response = "1";
    }
}
