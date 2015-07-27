/*
 * Copyright 2015 Alexander Rizaev
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

#ifndef ROCKET_CMS_TRANSLATIONMANAGER_H
#define ROCKET_CMS_TRANSLATIONMANAGER_H

#include <map>

#include <Poco/Path.h>

class TranslationManager {

public:
	static TranslationManager& getInstance();

	void load();
	const std::string& get(const std::string& key) const;

private:
	std::map<std::string, std::string> _store;

	void loadFile(const Poco::Path& filePath);
};

#endif
