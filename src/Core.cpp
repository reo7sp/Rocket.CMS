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

#include "Core.h"

#include "ConfigManager.h"
#include "TranslationManager.h"
#include "PageCompiler.h"
#include "CacheManager.h"
#include "admin/AdminServer.h"

Core& Core::get() {
	static Core instance;
	return instance;
}

void Core::start() {
	ConfigManager::get().reload();
	TranslationManager::get().reload();
	PageCompiler::get().start();
	AdminServer::get().start();
}

void Core::stop() {
	PageCompiler::get().stop();
	AdminServer::get().stop();
	CacheManager::get().clearAll();
}

int main() {
	Core::get().start();
}
