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

#include "CoreApp.h"

#include <stdio.h>
#include <iostream>

#include <Poco/TaskManager.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/SHA1Engine.h>

#include "rcms/web/WebHandlerFactory.h"
#include "rcms/PluginManager.h"

#include "tools/FsTools.h"
#include "TranslationManager.h"

using namespace Poco;
using namespace Poco::Util;
using namespace Poco::Net;

POCO_SERVER_MAIN(CoreApp)

int CoreApp::main(const std::vector<std::string>& args) {
    TranslationManager::getInstance().init();
    std::cout << TranslationManager::getInstance().get("hi");

	if (_canStart) {
		PluginManager::getInstance().load();
		if (!(_canStart = PluginManager::getInstance().onPreInit())) {
			logger().fatal("Error in some plugin's preinit method");
			_exitCode = EXIT_SOFTWARE;
		}
	}
	if (_canStart) {
        logger().information("Starting http server at port %i", (int) config().getUInt("web.port"));
		HTTPServer httpServer(new WebHandlerFactory, (UInt16) config().getUInt("web.port"));
		if (PluginManager::getInstance().onInit()) {
			httpServer.start();

			waitForTerminationRequest();

			logger().information("Stopping http server");
			httpServer.stopAll();
			PluginManager::getInstance().onDeinit();
		} else {
			logger().fatal("Error in some plugin's init method");
			_exitCode = EXIT_SOFTWARE;
		}
	}
	return _exitCode;
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
		Option("genconf", "", "generate config")
			.required(false)
			.repeatable(false)
			.callback(OptionCallback<CoreApp>(this, &CoreApp::handleGenConf))
	);
	options.addOption(
		Option("genhash", "", "generate SHA1 hash")
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
	if (!(_canStart = _canStart ? checkConfig() : false)) {
		_exitCode = EXIT_CONFIG;
	}
}

void CoreApp::handleGenConf(const std::string& name, const std::string& value) {
	const char* result = ""
		"{\n"
		"	\"fs\": {\n"
		"		\"root\": \"/path/to/your/awesome/site\",\n"
		"		\"site\": {\n"
		"			\"src\": \"src\",\n"
		"			\"dst\": \"dst\"\n"
		"		}\n"
		"	},\n"
		"	\"cms\": {\n"
		"		\"root\": \"/path/to/your/Rocket.CMS/installation/dir\"\n"
		"	},\n"
		"	\"web\": {\n"
		"		\"port\": 23307,\n"
		"		\"auth\": {\n"
		"			\"enabled\": false,\n"
		"			\"user\": \"admin\",\n"
		"			\"passhash\": \"d033e22ae348aeb5660fc2140aec35850c4da997\"\n"
		"		}\n"
		"	}\n"
		"}";
	printf(result);

	_canStart = false;
	stopOptionsProcessing();
}

void CoreApp::handleGenHash(const std::string& name, const std::string& value) {
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
	_exitCode = EXIT_USAGE;
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
	if (!config().has("cms.root")) {
		logger().fatal("cms.root must be set in the config");
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
	if (!config().has("cache.general.size")) {
		config().setUInt("cache.general.size", 256);
	}
	if (!config().has("cache.private.size")) {
		config().setUInt("cache.private.size", 128);
	}
	return result;
}
