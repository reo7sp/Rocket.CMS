/*
 * Copyright 2014 Reo_SP
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _ROCKETCMS_CONFIGMANAGER_H
#define _ROCKETCMS_CONFIGMANAGER_H

#include <string>
#include <boost/filesystem/path.hpp>

class ConfigManager {
public:
	static ConfigManager& getInstance();

	void reload();

	inline const std::string& getPort() const { return _port; }
	inline const std::string& getLang() const { return _lang; }
	inline const boost::filesystem::path& getSitePath() const { return _sitePath; }
	inline const std::string& getTitle() const { return _title; }
	inline const std::string& getTemplateToolkitCommand() const { return _templateToolkitCommand; }
	inline const std::string& getMarkdownCommand() const { return _markdownCommand; }

private:
	ConfigManager();
	ConfigManager(ConfigManager&);
	~ConfigManager();

	void operator=(ConfigManager&);

	std::string _port = "23488";
	std::string _lang = "en";
	boost::filesystem::path _sitePath = boost::filesystem::path("res/site");
	std::string _title = "Rocket.CMS";
	std::string _templateToolkitCommand = "tpage $1";
	std::string _markdownCommand = "markdown $1";
};

#endif
