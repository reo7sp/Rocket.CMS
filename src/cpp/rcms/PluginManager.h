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

#ifndef ROCKET_CMS_PLUGINMANAGER_H
#define ROCKET_CMS_PLUGINMANAGER_H

#include "FsEvent.h"

#include <vector>

#include "api/ApiConnection.h"
#include "pluginapi/rcms_Core.h"
#include "pluginapi/rcms_Plugin.h"

class PluginManager {

public:
    static void load();
    static void onPreInit();
    static void onInit();
    static void onDeinit();
    static void onFs(const FsEvent& event);
    static void onApi(ApiConnection& connection);

private:
    PluginManager() { }
    PluginManager(const PluginManager&) = delete;
    PluginManager& operator=(const PluginManager&) = delete;

    static std::vector<rcms_Plugin*> plugins;
    static rcms_Core core;
};


#endif //ROCKET_CMS_PLUGINMANAGER_H
