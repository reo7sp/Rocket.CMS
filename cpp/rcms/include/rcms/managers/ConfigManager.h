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

#ifndef ROCKETCMS_CONFIGMANAGER_H
#define ROCKETCMS_CONFIGMANAGER_H

#include <utility>

#include <Poco/Path.h>
#include <Poco/Util/AbstractConfiguration.h>

class ConfigManager {

public:
	ConfigManager(Poco::Util::AbstractConfiguration& _config) : _config(_config) { }

	std::pair<bool, std::vector<std::string>> checkAndFixConfig();

	Poco::Util::AbstractConfiguration& getConfig() const {
		return _config;
	}

	Poco::Path extractPath(const std::string& dirKey) const {
		return Poco::Path(getConfig().getString(dirKey));
	}

	Poco::Path extractPath(const std::string& rootDirKey, const std::string& dirKey) const {
		Poco::Path result(getConfig().getString(dirKey));
		if (result.isRelative()) {
			result.makeAbsolute(getPathFromConfig(rootDirKey));
		}
		return result;
	}

private:
	Poco::Util::AbstractConfiguration& _config;
};

#endif //ROCKETCMS_CONFIGMANAGER_H
