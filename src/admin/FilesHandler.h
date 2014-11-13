/*
 * Copyright 2014 Reo_SP
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _ROCKETCMS_ADMIN_FILESHANDLER_H
#define _ROCKETCMS_ADMIN_FILESHANDLER_H

#include "AbstractHandler.h"

class FilesHandler : public AbstractHandler {
public:
	static FilesHandler& get();

	virtual void displayEdit(mg_connection* connection) const override;
	virtual void displaySave(mg_connection* connection) const override;

private:
	FilesHandler() : AbstractHandler("files") {}
	FilesHandler(FilesHandler& a) : AbstractHandler(a) {}
	~FilesHandler() {}

	void operator=(FilesHandler&) {}
};

#endif
