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

#include "rcms/pluginapi/rcms_FsEvent.h"

const short RCMS_FSEVENT_NEW = 1;
const short RCMS_FSEVENT_EDIT = 2;
const short RCMS_FSEVENT_PUBLISH = 4;
const short RCMS_FSEVENT_MV = 8;
const short RCMS_FSEVENT_RM = 16;
