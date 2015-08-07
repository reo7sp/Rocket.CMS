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
	needRepaint: false
	syncModelTimeoutHandle: null
	autoSyncModelTimeoutHandle: null

	initialize: ->
		@model.get("file").set "isDir", false
		@el.classList.add "plain-text-file-editor"
		@el.setAttribute "placeholder", "#{ WebGUI.getStr "Enter some text here" }..."
		if @model.get("file").has "content"
			@el.contentEditable = true
			@render()
		else
			@el.innerHTML = "<br>"
			@model.loadFile()
				.then =>
					@el.contentEditable = true
					@render()
				.fail =>
					@el.contentEditable = true

	events:
		"input": (e) ->
			@model.fileIsDirty = true
			clearTimeout @syncModelTimeoutHandle
			@syncModelTimeoutHandle = setTimeout @syncModel.bind(@), 2500
			@autoSyncModelTimeoutHandle = setTimeout @syncModel.bind(@), 15000 if not @autoSyncModelTimeoutHandle?

		"paste": (e) ->
			@needRepaint = true
			setTimeout @syncModel.bind(@), 0

	render: ->
		c = @model.get("file").get "content"
			.trim()
			.replace /&/g, "&amp;"
			.replace /</g, "&lt;"
			.replace />/g, "&gt;"
			.replace /\"/g, "&quot;"
			.replace /'/g, "&#39;"
			.replace /\//g, "&#x2F;"
			.replace /\r\n/g, "<br>"
			.replace /\n/g, "<br>"
		@el.innerHTML = c

	syncModel: ->
		clearTimeout @autoSyncModelTimeoutHandle = @syncModelTimeoutHandle
		clearTimeout @syncModelTimeoutHandle
		@autoSyncModelTimeoutHandle = @syncModelTimeoutHandle = null

		c = @el.innerHTML
			.replace /\r/g, ""
			.replace /\n/g, ""
			.replace /<p>(.+?)<\/p>/g, "$1<br>\n"
			.replace /<br>/g, "\n"
			.replace /<.+?>/g, ""
			.replace /&lt;/g, "<"
			.replace /&gt;/g, ">"
			.replace /&quot;/g, "\""
			.replace /&#39;/g, "'"
			.replace /&#x2F;/g, "/"
			.replace /&amp;/g, "&"
			.trim()
		@el.innerHTML = "" if c == ""
		@model.get("file").set "content", c
		@model.fileIsDirty = false
		@model.saveFile()

		if @needRepaint
			@render()
			@needRepaint = false


module.exports.mimeType = /(text|application)\//

module.exports.title = WebGUI.getStr "Plain text"
