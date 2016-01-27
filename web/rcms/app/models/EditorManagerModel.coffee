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

Backbone = require "backbone"
WebGUI = require "../tools/WebGUI.coffee"

module.exports = Backbone.Model.extend
	defaults:
		file: null
		fileSaveStatus: ""
		saveTimeoutHandle: null

	saveFile: ->
		formatNumber = (number) ->
			s = number.toString()
			s = "0#{ s }" if number < 10

		f = =>
			@get("file").upload()
				.then =>
					d = new Date()
					@set "fileSaveStatus", "#{ WebGUI.getStr "Saved at"} #{ formatNumber d.getHours() }:#{ formatNumber d.getMinutes() }:#{ formatNumber d.getSeconds() }"
				.fail (reason) =>
					@set "fileSaveStatus", "#{ WebGUI.getStr "Can't save" }. #{ reason }"

		clearTimeout @get "saveTimeoutHandle"
		@set "saveTimeoutHandle", setTimeout(f, 2000)
