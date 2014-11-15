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
#include <stdexcept>
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
	_queueMutex.lock();
	_isRunning = false;
	_queueMutex.unlock();
}

void PageCompiler::compileAll() {
	const fs::path pagesDir = ConfigManager::get().getSitePath() / "pages";
	const fs::path filesDir = ConfigManager::get().getSitePath() / "files";

	fs::recursive_directory_iterator endIter;
	for (fs::recursive_directory_iterator iter(pagesDir); iter != endIter; ++iter) {
		compilePage(iter->path());
	}
	for (fs::recursive_directory_iterator iter(filesDir); iter != endIter; ++iter) {
		compileFile(iter->path());
	}
}

void PageCompiler::compilePage(const fs::path& file) {
	_queueMutex.lock();
	_queue.push(pair<int, fs::path>(QUEUE_PAGE, file));
	_queueMutex.unlock();
}

void PageCompiler::compileFile(const fs::path& file) {
	_queueMutex.lock();
	_queue.push(pair<int, fs::path>(QUEUE_FILE, file));
	_queueMutex.unlock();
}

void PageCompiler::threadAction() {
	const string publicDir = fs::absolute(ConfigManager::get().getSitePath() / "public").string();
	const string pagesDir = fs::absolute(ConfigManager::get().getSitePath() / "pages").string();
	const string filesDir = fs::absolute(ConfigManager::get().getSitePath() / "files").string();

	while (_isRunning) {
		_queueMutex.lock();
		if (_queue.empty()) {
			_queueMutex.unlock();
			this_thread::sleep_for(chrono::seconds(1));
		} else {
			const pair<int, fs::path> item = _queue.front();
			const int type = item.first;
			const fs::path filePath = item.second;
			_queue.pop();
			_queueMutex.unlock();

			try {
				string file = fs::absolute(filePath).string();
				if (type == QUEUE_PAGE) {
					string text = Utils::readFile(file);
					replace_first(file, pagesDir, publicDir);
					Utils::saveFile(file, text);

					compileMarkdown(file);
					compileTemplateToolkit(file);
				} else if (type == QUEUE_FILE) {
					fs::path publicPath = fs::path(replace_first_copy(file, filesDir, publicDir));
					fs::create_directories(publicPath.parent_path());
					fs::remove(publicPath);
					fs::copy(file, publicPath);
				}
			} catch (const exception& e) {
				Log::error("Exception in compiler thread: " + string(e.what()));
			} catch (...) {
				Log::error("Unknown exception in compiler thread");
			}
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

	fs::current_path(ConfigManager::get().getSitePath() / "template");
	const string out = Utils::exec(replace_all_copy(ConfigManager::get().getTemplateToolkitCommand(), "$1", file));
	fs::current_path(oldCurDir);

	Utils::saveFile(file, out);
}
