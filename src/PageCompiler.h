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

#ifndef _ROCKETCMS_GENERATOR_H
#define _ROCKETCMS_GENERATOR_H

#include <string>
#include <queue>
#include <mutex>
#include <boost/filesystem/path.hpp>

class PageCompiler {
public:
	static PageCompiler& getInstance();

	void start();
	void stop();
	void compile(const boost::filesystem::path& file);

	inline bool isRunning() const { return _isRunning; }

private:
	PageCompiler();
	PageCompiler(PageCompiler&);
	~PageCompiler();

	void operator=(PageCompiler&);
	void threadAction();
	void compileMarkdown(const boost::filesystem::path& file) const;
	void compileTemplateToolkit(const boost::filesystem::path& file) const;

	std::queue<boost::filesystem::path> _queue;
	std::mutex _queueMutex;
	bool _isRunning;
};

#endif