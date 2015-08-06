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

formatNumber = (number) ->
	s = number.toString()
	s = "0#{ s }" if number < 10
	s

doSomeThingWithFile = (actionFunc, verbString) ->
	verbInNotInfinitiveForm = if verbString[verbString.length - 1] == "e" then verbString.substring(0, verbString.length - 1) else verbString

	@set "fileStatus", "#{ WebGUI.getStr "#{ verbInNotInfinitiveForm }ing" }..."
	actionFunc()
		.progress (percent) =>
			@set "fileStatus", "#{ WebGUI.getStr "#{ verbInNotInfinitiveForm }ing" }... #{ Math.round(percent * 100) }%"

		.then (data) =>
			d = new Date()
			@set "fileStatus", "#{ WebGUI.getStr "#{ verbInNotInfinitiveForm }ed at"} #{ formatNumber d.getHours() }:#{ formatNumber d.getMinutes() }:#{ formatNumber d.getSeconds() }"
			data

		.fail (err) =>
			@set "fileStatus", "#{ WebGUI.getStr "Can't #{ verbString.toLowerCase() }" }. #{ WebGUI.getStr err.message }"
			err

module.exports = rcms.models.EditorModel = Backbone.Model.extend
	defaults:
		file: null
		fileSaveStatus: ""

	loadFile: ->
		doSomeThingWithFile.bind(@, @get("file").load.bind(@get "file"), "Load")()

	saveFile: ->
		doSomeThingWithFile.bind(@, @get("file").upload.bind(@get "file"), "Save")()

	publishFile: ->
		doSomeThingWithFile.bind(@, @get("file").publish.bind(@get "file"), "Publish")()
