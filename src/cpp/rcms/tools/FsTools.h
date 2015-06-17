/*
 * Copyright 2015 Reo_SP
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ROCKET_CMS_FSTOOLS_H
#define ROCKET_CMS_FSTOOLS_H

#include <fstream>
#include <string>

#include <Poco/Path.h>
#include <Poco/SharedPtr.h>

#include "../CacheManager.h"

namespace FsTools {

    void loadFileToString(const Poco::Path& path, Poco::SharedPtr<std::string>& buffer, bool useCache = true);

    inline void loadFileToString(const std::string& path, Poco::SharedPtr<std::string>& buffer, bool useCache = true) {
        loadFileToString(Poco::Path(path), buffer);
    }

    inline Poco::SharedPtr<std::string> loadFileToString(const Poco::Path& path, bool useCache = true) {
        Poco::SharedPtr<std::string> result;
        loadFileToString(path, result);
        return result;
    }

    inline Poco::SharedPtr<std::string> loadFileToString(const std::string& path, bool useCache = true) {
        return loadFileToString(Poco::Path(path), useCache);
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

}

#endif //ROCKET_CMS_FSTOOLS_H
