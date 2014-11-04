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

#include "AdminServer.h"

#include <string>
#include <iostream>

using namespace std;

AdminServer::AdminServer() {
}

AdminServer::~AdminServer() {
	stop();
}

AdminServer& AdminServer::getInstance() {
	static AdminServer instance;
	return instance;
}

void AdminServer::start() {
	if (_isRunning) return;
	_isRunning = true;

	cout << "Starting server at port 23488" << endl;
	_server = mg_create_server(nullptr, &AdminServer::handleEvent);
	mg_set_option(_server, "listening_port", "23488");
	while (_isRunning) {
		mg_poll_server(_server, 1000);
	}
	mg_destroy_server(&_server);
	cout << "Server has been stopped" << endl;
	_server = nullptr;
}

void AdminServer::stop() {
	_isRunning = false;
}

int AdminServer::handleEvent(mg_connection* connection, mg_event event) {
	if (event == MG_AUTH) {
		return MG_TRUE;
	} else if (event == MG_REQUEST) {
		return MG_FALSE;
	} else {
		return MG_FALSE;
	}
}
