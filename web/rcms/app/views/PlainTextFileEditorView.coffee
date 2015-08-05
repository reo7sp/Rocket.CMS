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

module.exports = Backbone.View.extend
	initialize: ->
		@model.get("file").set "isDir", false
		@el.setAttribute "placeholder", "#{ WebGUI.getStr "Enter some text here" }..."
		@el.classList.add "plain-text-editor"
		if @model.get("file").has "content"
			@el.contentEditable = true
			@render()
		else
			@el.innerHTML = "#{ WebGUI.getStr "Loading file" }..."
			@model.get("file").load()
				.then =>
					@el.contentEditable = true
					@render()
				.fail =>
					@el.innerHTML = WebGUI.getStr "File loading failed"
				.done()

	events:
		"input": ->
			if @el.innerHTML == "<br>"
				@el.innerHTML = ""
			@model.get("file").set "content", @el.innerHTML
			@model.saveFile()

	render: ->
		@el.innerHTML = @model.get("file").get "content"

module.exports.mimeType = /(text|application)\//

module.exports.title = WebGUI.getStr "Plain text"
