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

#ifndef _ROCKETCMS_TRANSLATIONMANAGER_H
#define _ROCKETCMS_TRANSLATIONMANAGER_H

#include <string>
#include <map>

class TranslationManager {
public:
	static TranslationManager& get();

	void reload();
	std::string getString(const std::string& key) const;
	void translate(std::string& text) const;

private:
	TranslationManager();
	TranslationManager(TranslationManager&);
	~TranslationManager();

	void operator=(TranslationManager&);

	std::map<std::string, std::string> _data;
};

#endif
