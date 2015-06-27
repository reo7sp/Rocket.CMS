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

#ifndef ROCKET_CMS_RCMS_FSEVENT_H
#define ROCKET_CMS_RCMS_FSEVENT_H

#include <stddef.h>

extern "C" {

extern const short RCMS_FSEVENT_NEW;
extern const short RCMS_FSEVENT_EDIT;
extern const short RCMS_FSEVENT_PUBLISH;
extern const short RCMS_FSEVENT_MV;
extern const short RCMS_FSEVENT_RM;

struct rcms_FsEvent {
    const short type = 0;
    const char* file1Path = NULL;
    const char* file2Path = NULL;
};

};

#endif //ROCKET_CMS_RCMS_FSEVENT_H
