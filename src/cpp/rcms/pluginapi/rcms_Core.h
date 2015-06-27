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

#ifndef ROCKET_CMS_RCMS_CORE_H
#define ROCKET_CMS_RCMS_CORE_H

#include "rcms_ApiConnection.h"

extern "C" {

void rcms_Core_invokeApiCall(const rcms_ApiConnection* connection);
void rcms_Core_saveToCache(const char* name, const char* value);
const char* rcms_Core_getFromCache(const char* name);
void rcms_Core_invalidateCacheItem(const char* name);
const char* rcms_Core_loadFile(const char* name);

struct rcms_Core {
    void (*const invokeApiCall)(const rcms_ApiConnection*) = &rcms_Core_invokeApiCall;
    void (*const saveToCache)(const char*, const char*) = &rcms_Core_saveToCache;
    const char* (*const getFromCache)(const char*) = &rcms_Core_getFromCache;
    void (*const invalidateCacheItem)(const char*) = &rcms_Core_invalidateCacheItem;
    const char* (*const loadFile)(const char*) = &rcms_Core_loadFile;
};

};

#endif //ROCKET_CMS_RCMS_CORE_H
