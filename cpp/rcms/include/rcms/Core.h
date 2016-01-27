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

#ifndef ROCKETCMS_CORE_H
#define ROCKETCMS_CORE_H

#include <Poco/Logger.h>

#include "rcms/ConfigManager.h"
#include "rcms/PluginManager.h"

class Core {

public:
	Core(ConfigManager& configManager, PluginManager& pluginsManager, Poco::Logger& logger) : _configManager(config), _pluginManager(pluginManager), _logger(logger) { }

	ConfigManager& getConfigManager() {
		return _configManager;
	}

	PluginManager& getPluginManager() {
		return _pluginManager;
	}

	Poco::Logger& getLogger() const {
		return _logger;
	}

private:
	ConfigManager& _configManager;
	PluginManager& _pluginManager;
	Poco::Logger& _logger;
};

#endif //ROCKETCMS_CORE_H
