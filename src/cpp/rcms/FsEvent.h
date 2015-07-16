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

#ifndef ROCKET_CMS_FSEVENT_H
#define ROCKET_CMS_FSEVENT_H

#include <string>

#include <Poco/Path.h>

class FsEvent {

public:
	enum Type {
		NEW = 1,
		EDIT = 2,
		PUBLISH = 4,
		MV = 8,
		RM = 16
	};

	FsEvent(const Type& type, const std::string& file1Path, const std::string& file2Path);
	FsEvent(const Type& type, const std::string& file1Path);
	FsEvent(const Type& type, const Poco::Path& file1Path, const Poco::Path& file2Path);
	FsEvent(const Type& type, const Poco::Path& file1Path);

	Type type;
	std::string file1Path;
	std::string file2Path;
};

#endif //ROCKET_CMS_FSEVENT_H
