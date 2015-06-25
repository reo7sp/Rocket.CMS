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

#include <vector>

#include <Poco/Util/Application.h>
#include <Poco/Path.h>
#include <Poco/File.h>

#include "../../tools/FsTools.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::Util;

bool FsApiHandler::isHandlerNameEquals(const string& handlerName) {
    return handlerName == "fs";
}

void FsApiHandler::handleRequest(const string& methodName, const map<string, string>& args,
                                 HTTPServerRequest& request, HTTPServerResponse& response) {
    if (methodName == "ls") {
        Path dirPath(FsTools::getPathFromConfig("fs.site.src"), args.at("dir"));
        vector<File> files;
        File(dirPath).list(files);

        string jsonResponseStr;
        jsonResponseStr += '[';
        size_t lastSlashPos = dirPath.toString(Path::PATH_UNIX).rfind('/');
        for (File item : files) {
            jsonResponseStr += "{name:\"";
            jsonResponseStr += Path(item.path().substr(lastSlashPos + 1)).toString(Path::PATH_UNIX);
            jsonResponseStr += "\",isdir:";
            jsonResponseStr += item.isDirectory() ? '1' : '0';
            jsonResponseStr += "},";
        }
        jsonResponseStr.erase(jsonResponseStr.length() - 2);
        jsonResponseStr += ']';

        response.setContentType("application/json");
        response.send() << jsonResponseStr;
    } else if (methodName == "getfile") {
        Path filePath(FsTools::getPathFromConfig("fs.site.src"), args.at("file"));

        response.sendFile(filePath.toString(Path::PATH_UNIX), FsTools::getMimeType(filePath));
    } else if (methodName == "getmeta") {
        // TODO
    } else if (methodName == "rm") {
        Path filePath(FsTools::getPathFromConfig("fs.site.src"), args.at("file"));
        Path fileDestPath(FsTools::getPathFromConfig("fs.site.dst"), args.at("file"));

        File(filePath).remove(true);
        File fileDestFile(fileDestPath);
        if (fileDestFile.exists()) {
            fileDestFile.remove(true);
        }

        response.send() << '1';
    } else if (methodName == "mv") {
        Path fromPath(FsTools::getPathFromConfig("fs.site.src"), args.at("from"));
        Path toPath(FsTools::getPathFromConfig("fs.site.src"), args.at("to"));

        File(fromPath).moveTo(toPath.toString());

        response.send() << '1';
    } else if (methodName == "upload") {
        // TODO
    } else if (methodName == "edit") {
        // TODO
    } else if (methodName == "editmeta") {
        // TODO
    } else if (methodName == "publish") {
        // TODO
    }
}
