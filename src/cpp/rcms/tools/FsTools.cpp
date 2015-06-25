/*
 * Copyright 2015 Reo_SP,
 *           2015 stroum
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

#include "FsTools.h"

namespace FsTools {

    void loadFileToString(const Poco::Path& path, Poco::SharedPtr<std::string>& buffer, bool useCache = true) {
        std::string pathString = path.toString();
        if (useCache) {
            buffer = CacheManager::getFsCache().get(pathString);
            if (buffer->size() > 0) {
                return;
            }
        }
        std::ifstream input(path.toString());
        input >> *buffer;
        if (useCache) {
            CacheManager::getFsCache().add(pathString, buffer);
        }
    }

    std::string getMimeType(const std::string& name) {
        static const std::map<std::string, std::string> mimeTypes = {
            {"gif",   "image/gif"},
            {"jpg",   "image/jpeg"},
            {"jpeg",  "image/jpeg"},
            {"png",   "image/png"},
            {"bmp",   "image/bmp"},
            {"svg",   "image/svg+xml"},
            {"tiff",  "image/tiff"},
            {"js",    "application/javascript"},
            {"ogg",   "application/ogg"},
            {"pdf",   "application/pdf"},
            {"rss",   "application/rss+xml"},
            {"woff",  "application/font-woff"},
            {"xhtml", "application/xhtml+xml"},
            {"xml",   "application/xml"},
            {"zip",   "application/zip"},
            {"gz",    "application/gzip"},
            {"mp3",   "audio/mpeg"},
            {"ogg",   "audio/ogg"},
            {"flac",  "audio/flac"},
            {"opus",  "audio/opus"},
            {"wave",  "audio/vnd.wave"},
            {"css",   "text/css"},
            {"csv",   "text/csv"},
            {"html",  "text/html"},
            {"md",    "text/markdown"},
            {"rtf",   "text/rtf"},
            {"avi",   "video/avi"},
            {"mpeg",  "video/mpeg"},
            {"mp4",   "video/mp4"},
            {"webm",  "video/webm"},
            {"mkv",   "video/x-matroska"},
            {"flv",   "video/x-flv"}
        };
        auto iter = mimeTypes.find(name);
        if (iter == mimeTypes.end()) {
            return "text/plain";
        }
        return iter->second;
    }

}
