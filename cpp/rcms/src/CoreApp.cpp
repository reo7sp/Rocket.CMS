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

#include "rcms/CoreApp.h"

#include <iostream>

#include <Poco/SHA1Engine.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/FormattingChannel.h>
#include <Poco/PatternFormatter.h>

#include "rcms/PluginManager.h"
#include "rcms/CacheManager.h"
#include "rcms/tools/FsTools.h"
#include "rcms/TranslationManager.h"
#include "rcms/web/WebHandlerFactory.h"

using namespace Poco;
using namespace Poco::Util;
using namespace Poco::Net;

int CoreApp::main(const std::vector<std::string>& args) {
	logger().setChannel(new FormattingChannel(new PatternFormatter("%L%H:%M:%S.%i [%q] %t"), logger().getChannel()));
	if (config().getBool("debug")) {
		logger().setLevel(Message::PRIO_TRACE);
		config().setUInt("cache.general.size", 0);
		config().setUInt("cache.private.size", 0);
		logger().warning("Debug mode is on");
	}
	if (_canStart) {
		logger().information("All options processed. Starting Rocket.CMS");
		CacheManager::getInstance().init();

		logger().information("Loading translations");
		TranslationManager::getInstance().load();

		logger().information("Loading plugins");
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

			logger().information("Rocket.CMS has started successfully");
			waitForTerminationRequest();

			logger().information("Stopping http server");
			httpServer.stopAll();
			PluginManager::getInstance().onDeinit();
			logger().information("Everything has stopped");
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
			.argument("jsonfile")
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
	options.addOption(
		Option("debug", "d", "turns on debug mode")
			.repeatable(false)
			.callback(OptionCallback<CoreApp>(this, &CoreApp::handleDebug))
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
		"		\"site\": {\n"
		"			\"root\": \"/path/to/your/awesome/site\",\n"
		"			\"src\": \"src\",\n"
		"			\"dst\": \"dst\",\n"
		"			\"defaultFileExtention\": \"md\"\n"
		"		},\n"
		"		\"cms\": {\n"
		"			\"root\": \"/path/to/your/Rocket.CMS/installation/dir\"\n"
		"		}\n"
		"	},\n"
		"	\"web\": {\n"
		"		\"lang\": \"en\",\n"
		"		\"port\": 23307,\n"
		"		\"auth\": {\n"
		"			\"enabled\": false,\n"
		"			\"user\": \"admin\",\n"
		"			\"passhash\": \"d033e22ae348aeb5660fc2140aec35850c4da997\"\n"
		"		}\n"
		"	}\n"
		"}\n";
	printf("%s", result);

	_canStart = false;
	stopOptionsProcessing();
}

void CoreApp::handleGenHash(const std::string& name, const std::string& value) {
	SHA1Engine sha1Engine;
	sha1Engine.update(value);
	printf("%s", Poco::DigestEngine::digestToHex(sha1Engine.digest()).c_str());

	_canStart = false;
	stopOptionsProcessing();
}

void CoreApp::handleHelp(const std::string& name, const std::string& value) {
	HelpFormatter helpFormatter(options());
	helpFormatter.setCommand(commandName());
	helpFormatter.format(std::cout);

	_canStart = false;
	_exitCode = EXIT_USAGE;
	stopOptionsProcessing();
}

void CoreApp::handleDebug(const std::string& name, const std::string& value) {
	config().setBool("debug", true);
}

bool CoreApp::checkConfig() {
	bool result = true;
	if (!config().has("fs.site.root")) {
		logger().fatal("fs.site.root must be set in the config");
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
	if (!config().has("fs.site.defaultFileExtention")) {
		logger().warning("fs.site.defaultFileExtention is set to \"md\"");
		config().setString("fs.site.defaultFileExtention", "md");
	}
	if (!config().has("fs.cms.root")) {
		logger().fatal("fs.cms.root must be set in the config");
		result = false;
	}
	if (!config().has("web.lang")) {
		logger().warning("web.lang is set to \"en\"");
		config().setString("web.lang", "en");
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
		config().setUInt("cache.general.size", 128);
	}
	if (!config().has("cache.private.size")) {
		config().setUInt("cache.private.size", 32);
	}
	if (!config().has("debug")) {
		config().setBool("debug", false);
	}
	return result;
}
