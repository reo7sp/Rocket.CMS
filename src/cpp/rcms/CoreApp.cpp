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

#include "CoreApp.h"

#include <iostream>

#include <Poco/TaskManager.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/SHA1Engine.h>
#include <stdio.h>

#include "web/WebHandlerFactory.h"

using namespace Poco;
using namespace Poco::Util;
using namespace Poco::Net;

POCO_SERVER_MAIN(CoreApp)

int CoreApp::main(const std::vector<std::string>& args) {
    if (_canStart) {
        HTTPServer httpServer(new WebHandlerFactory, (UInt16) config().getUInt("web.port"));
        logger().information("Starting http server at port %i", config().getUInt("web.port"));
        httpServer.start();
        waitForTerminationRequest();
        logger().information("Stopping http server");
        httpServer.stopAll();
    }
    return Application::EXIT_OK;
}

void CoreApp::defineOptions(OptionSet& options) {
    ServerApplication::defineOptions(options);

    options.addOption(
        Option("config", "c", "specify Rocket.CMS configuration file")
            .required(true)
            .repeatable(false)
            .argument("json file")
            .callback(OptionCallback<CoreApp>(this, &CoreApp::handleConfig))
    );
    options.addOption(
        Option("genhash", "g", "generate SHA1 hash")
            .required(false)
            .repeatable(false)
            .argument("string")
            .callback(OptionCallback<CoreApp>(this, &CoreApp::handleGenHash))
    );
    options.addOption(
        Option("help", "h", "display help information on command line arguments")
            .required(false)
            .repeatable(false)
            .callback(OptionCallback<CoreApp>(this, &CoreApp::handleHelp))
    );
}

void CoreApp::handleConfig(const std::string& name, const std::string& value) {
    loadConfiguration(value);
    _canStart = _canStart ? checkConfig() : false;
}

void CoreApp::handleGenHash(const std::string& name, const std::string& value){
    SHA1Engine sha1Engine;
    sha1Engine.update(value);
    printf(Poco::DigestEngine::digestToHex(sha1Engine.digest()).c_str());

    _canStart = false;
    stopOptionsProcessing();
}

void CoreApp::handleHelp(const std::string& name, const std::string& value) {
    HelpFormatter helpFormatter(options());
    helpFormatter.setCommand(commandName());
    helpFormatter.setUsage("OPTIONS");
    helpFormatter.setHeader("Rocket.CMS");
    helpFormatter.format(std::cout);

    _canStart = false;
    stopOptionsProcessing();
}

bool CoreApp::checkConfig() {
    bool result = true;
    if (!config().has("fs.root")) {
        logger().fatal("fs.root must be set in the config");
        result = false;
    }
    if (!config().has("fs.site.src")) {
        logger().fatal("fs.site.src must be set in the config");
        result = false;
    }
    if (!config().has("fs.site.dst")) {
        logger().fatal("fs.site.dst must be set in the config");
        result = false;
    }
    if (!config().has("web.port")) {
        logger().warning("web.port is set to 23307");
        config().setUInt("web.port", 23307);
    }
    if (!config().has("web.auth.enabled")) {
        logger().warning("web.auth.enabled is set to false");
        config().setBool("web.auth.enabled", false);
    } else if (config().getBool("web.auth.enabled")) {
        if (!config().has("web.auth.user")) {
            logger().fatal("web.auth.user must be set in the config");
            result = false;
        }
        if (!config().has("web.auth.passhash")) {
            logger().fatal("web.auth.passhash must be set in the config");
            result = false;
        }
    }
    if (!config().has("cache.fs.size")) {
        config().setUInt("cache.fs.size", 64);
    }
    return result;
}
