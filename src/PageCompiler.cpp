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

#include "PageCompiler.h"

#include "Log.h"
#include "Utils.h"
#include "ConfigManager.h"

#include <thread>
#include <chrono>
#include <boost/algorithm/string/replace.hpp>

using namespace std;
using namespace boost::algorithm;
namespace fs = boost::filesystem;

PageCompiler::PageCompiler() {
}

PageCompiler::~PageCompiler() {
	stop();
}

PageCompiler& PageCompiler::getInstance() {
	static PageCompiler instance;
	return instance;
}

void PageCompiler::start() {
	if (_isRunning) return;
	_isRunning = true;

	thread compileThread(&PageCompiler::threadAction, this);
	compileThread.detach();
	Log::info("Page compile thread has been started");
}

void PageCompiler::stop() {
	_isRunning = false;
}

void PageCompiler::compile(const fs::path& file) {
	_queueMutex.lock();
	_queue.push(file);
	_queueMutex.unlock();
}

void PageCompiler::threadAction() {
	while (_isRunning) {
		_queueMutex.lock();
		if (_queue.empty()) {
			_queueMutex.unlock();
			this_thread::sleep_for(chrono::seconds(1));
		} else {
			fs::path file = _queue.front();
			_queue.pop();
			_queueMutex.unlock();

			string text = Utils::readFile(file);
			fs::path outputPath = ConfigManager::getInstance().getSitePath() / "public" / (file.filename().stem().string() + ".html");
			Utils::saveFile(outputPath, text);

			compileMarkdown(outputPath);
			compileTemplateToolkit(outputPath);
		}
	}
}

void PageCompiler::compileMarkdown(const fs::path& file) const {
	string out = Utils::exec(replace_all_copy(ConfigManager::getInstance().getMarkdownCommand(), "$1", file.string()));
	Utils::saveFile(file, out);
}

void PageCompiler::compileTemplateToolkit(const fs::path& file) const {
	string out = Utils::exec(replace_all_copy(ConfigManager::getInstance().getTemplateToolkitCommand(), "$1", file.string()));
	Utils::saveFile(file, out);
}