# Copyright 2015 Oleg Morozenkov
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#	http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

Q = require "q"

module.exports = rcms.types.tools.Net =
	request: (method, path, data) ->
		Q.Promise (resolve, reject, notify) ->
			handle = new XMLHttpRequest()

			handle.onload = ->
				if handle.status == 200
					resolve handle.response
				else
					reject new Error "#{ handle.status }. #{ handle.statusText }"

			handle.onerror = ->
				reject new Error "Can't establish connection"

			handle.onprogress = (e) ->
				notify e.loaded / e.total if e.lengthComputable

			handle.open method, path, true
			handle.send data

			return

	get: (path) ->
		@request "GET", path, null

	post: (path, data) ->
		@request "POST", path, data
