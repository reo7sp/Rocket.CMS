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
#include <boost/filesystem/operations.hpp>

using namespace std;
using namespace boost::algorithm;
namespace fs = boost::filesystem;

PageCompiler::PageCompiler() {
}

PageCompiler::~PageCompiler() {
	stop();
}

PageCompiler& PageCompiler::get() {
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
	const string pagesDir = fs::absolute(ConfigManager::get().getSitePath() / "pages").string();
	const string publicDir = fs::absolute(ConfigManager::get().getSitePath() / "public").string();
	while (_isRunning) {
		_queueMutex.lock();
		if (_queue.empty()) {
			_queueMutex.unlock();
			this_thread::sleep_for(chrono::seconds(1));
		} else {
			const fs::path filePath = _queue.front();
			_queue.pop();
			_queueMutex.unlock();

			string file = fs::absolute(filePath).string();
			string text = Utils::readFile(file);
			replace_first(file, pagesDir, publicDir);
			Utils::saveFile(file, text);

			compileMarkdown(file);
			compileTemplateToolkit(file);
		}
	}
}

void PageCompiler::compileMarkdown(const string& file) const {
	string out = Utils::exec(replace_all_copy(ConfigManager::get().getMarkdownCommand(), "$1", file));
	replace_all(out, "<p>[%", "[%");
	replace_all(out, "%]</p>", "%]");
	Utils::saveFile(file, out);
}

void PageCompiler::compileTemplateToolkit(const string& file) const {
	const fs::path oldCurDir = fs::current_path();

	fs::current_path(ConfigManager::get().getSitePath());
	const string out = Utils::exec(replace_all_copy(ConfigManager::get().getTemplateToolkitCommand(), "$1", file));
	fs::current_path(oldCurDir);

	Utils::saveFile(file, out);
}
