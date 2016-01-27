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

#ifndef ROCKET_CMS_LAUNCHER_H
#define ROCKET_CMS_LAUNCHER_H

#include <Poco/Util/ServerApplication.h>
#include <Poco/Net/HTTPServer.h>

class Launcher : public Poco::Util::ServerApplication {

protected:
	virtual int main(const std::vector<std::string>& args) override;
	virtual void defineOptions(Poco::Util::OptionSet &options) override;

private:
	bool _canStart = true;
	ExitCode _exitCode = EXIT_OK;

	void handleConfig(const std::string& name, const std::string& value);
	void handleGenConf(const std::string& name, const std::string& value);
	void handleGenHash(const std::string& name, const std::string& value);
	void handleHelp(const std::string& name, const std::string& value);
	void handleDebug(const std::string& name, const std::string& value);
};

#endif //ROCKET_CMS_LAUNCHER_H
