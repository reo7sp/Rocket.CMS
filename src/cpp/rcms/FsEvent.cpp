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

#include "FsEvent.h"

FsEvent::FsEvent(const FsEvent::Type& type, const std::string& file1Path, const std::string& file2Path)
	: type(type), file1Path(file1Path), file2Path(file2Path) {
}

FsEvent::FsEvent(const FsEvent::Type& type, const std::string& file1Path)
	: type(type), file1Path(file1Path) {
}

FsEvent::FsEvent(const FsEvent::Type& type, const Poco::Path& file1Path, const Poco::Path& file2Path)
	: type(type), file1Path(file1Path.toString()), file2Path(file2Path.toString()) {
}

FsEvent::FsEvent(const FsEvent::Type& type, const Poco::Path& file1Path)
	: type(type), file1Path(file1Path.toString()) {
}

