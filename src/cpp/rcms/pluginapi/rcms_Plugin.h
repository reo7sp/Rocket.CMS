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

#ifndef ROCKET_CMS_RCMS_PLUGIN_H
#define ROCKET_CMS_RCMS_PLUGIN_H

#include <stddef.h>

#include "rcms_Core.h"
#include "rcms_ApiConnection.h"
#include "rcms_FsEvent.h"

extern "C" {

struct rcms_Plugin {
	char* apiHandlerName = NULL;
	bool (*handleApi)(const rcms_Core*, const rcms_ApiConnection*) = NULL;
	bool (*onPreInit)(const rcms_Core*) = NULL;
	bool (*onInit)(const rcms_Core*) = NULL;
	void (*onDeinit)(const rcms_Core*) = NULL;
	void (*onFsEvent)(const rcms_Core*, const rcms_FsEvent*) = NULL;
};

};

#endif //ROCKET_CMS_RCMS_PLUGIN_H
