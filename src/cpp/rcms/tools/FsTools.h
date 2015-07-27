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
    std::ifstream stream(path.toString());
    return std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
}

inline std::string loadFileToString(const std::string& path) {
	return loadFileToString(Poco::Path(path));
}

inline bool writeStringToFile(const Poco::Path& path, std::string& data) {
	std::ofstream stream(path.toString());
	stream << data;
	stream.close();
	return !stream.fail();
}

inline bool writeStringToFile(const std::string& path, std::string& data) {
	return writeStringToFile(Poco::Path(path), data);
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
