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

#ifndef _ROCKETCMS_ADMIN_ABSTRACTHANDLER_H
#define _ROCKETCMS_ADMIN_ABSTRACTHANDLER_H

#include <string>
#include <mongoose/mongoose.h>

class AbstractHandler {
public:
	virtual bool tryDisplay(mg_connection* connection) const;
	virtual void displayList(mg_connection* connection) const;
	virtual void displayEdit(mg_connection* connection) const;
	virtual void displaySave(mg_connection* connection) const;
	virtual void displayDelete(mg_connection* connection) const;
	virtual void displayCompileAll(mg_connection* connection) const;

protected:
	AbstractHandler(const std::string& name) : _name(name) {}

	std::string _name;
};

#endif
