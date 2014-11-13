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

#ifndef _ROCKETCMS_ADMIN_PAGESHANDLER_H
#define _ROCKETCMS_ADMIN_PAGESHANDLER_H

#include <mongoose/mongoose.h>

class PagesHandler {
public:
	static PagesHandler& get();

	bool tryDisplay(mg_connection* connection);
	void displayPagesList(mg_connection* connection);
	void displayPagesEdit(mg_connection* connection);
	void displayPagesSave(mg_connection* connection);
	void displayPagesDelete(mg_connection* connection);

private:
	PagesHandler();
	PagesHandler(PagesHandler&);
	~PagesHandler();

	void operator=(PagesHandler&);
};

#endif
