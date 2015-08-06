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
Net = require "./Net.coffee"

cacheFiles = {}

rcms.ui.updateListeners.push ->
	for item in document.getElementsByClassName("_WebGUI.getStr")
		item.innerHTML = module.exports.getStr item.getAttribute "data-webgui-getstr"

module.exports = rcms.tools.WebGUI =
	getFile: (key) ->
		result = cacheFiles[key]
		if result?
			if result.then?
				result
			else
				Q.fcall ->
					result
		else
			p = Net.get "/api/webgui/getfile?file=#{ key }"
			cacheFiles[key] = p
			p.then (value) ->
				cacheFiles[key] = value
			p.fail (err) ->
				cacheFiles[key] = null
				throw err

	getStr: (key) ->
		key # TODO

