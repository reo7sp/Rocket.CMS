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

#ifndef ROCKET_CMS_FSTOOLS_H
#define ROCKET_CMS_FSTOOLS_H

#include <string>
#include <fstream>

#include <Poco/Path.h>
#include <Poco/SharedPtr.h>
#include <Poco/Util/Application.h>

namespace FsTools {

inline std::string loadFileToString(const Poco::Path& path) {
	std::string result;
	std::ifstream input(path.toString());
	input >> result;
	input.close();
	return result;
}

inline std::string loadFileToString(const std::string& path) {
	return loadFileToString(Poco::Path(path));
}

inline Poco::Path getPathFromConfig(const std::string& name) {
	Poco::Path result(Poco::Util::Application::instance().config().getString(name));
	if (result.isRelative()) {
		result.makeAbsolute(getPathFromConfig("fs.root"));
	}
	return result;
}

std::string getMimeType(const std::string& name);

inline std::string getMimeType(const Poco::Path& path) {
	return getMimeType(path.getExtension());
}

std::string concat(const std::vector<Poco::Path>& files, const std::string& separator = "\r\n");

std::string concat(const std::vector<std::string>& files, const std::string& separator = "\r\n");

}

#endif //ROCKET_CMS_FSTOOLS_H
