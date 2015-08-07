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
marked = require "marked"
toMarkdown = require "to-markdown"

module.exports = Backbone.View.extend
	needRepaint: false
	syncModelTimeoutHandle: null
	autoSyncModelTimeoutHandle: null
	mainEl: null

	initialize: ->
		rcms.tools.WebGUI.getFile "plugins/markdownWysiwygEditor/templates/mwfe.html"
			.then (data) =>
				@initMarkup data
			.done()

	events:
		"input .mwfe__main": (e) ->
			@model.fileIsDirty = true
			clearTimeout @syncModelTimeoutHandle
			@syncModelTimeoutHandle = setTimeout @syncModel.bind(@), 2500
			@autoSyncModelTimeoutHandle = setTimeout @syncModel.bind(@), 15000 if not @autoSyncModelTimeoutHandle?

		"paste .mwfe__main": (e) ->
			@needRepaint = true
			setTimeout @syncModel.bind(@), 0

	render: ->
		renderer = new marked.Renderer()
		renderer.html = (html) ->
			result = html
			result = result.replace "class=\"", "class=\"md-initial-html "
			result = result.replace "class='", "class='md-initial-html "
			result = result.replace /(<.+? )/gi , "$1 class=\"md-initial-html\" " if result == html
			result

		@mainEl.innerHTML = marked @model.get("file").get("content"), { renderer: renderer }

	initMarkup: (html) ->
		@el.innerHTML = html
		rcms.ui.update()
		@mainEl = @el.getElementsByClassName("mwfe__main")[0]
		if @model.get("file").has "content"
			@render()
		else
			@mainEl.contentEditable = false
			@mainEl.innerHTML = "<br>"
			@model.loadFile()
				.then =>
					@mainEl.contentEditable = true
					@render()
				.fail =>
					@mainEl.contentEditable = true

	syncModel: ->
		clearTimeout @autoSyncModelTimeoutHandle = @syncModelTimeoutHandle
		clearTimeout @syncModelTimeoutHandle
		@autoSyncModelTimeoutHandle = @syncModelTimeoutHandle = null

		toMarkdownOptions =
			gfm: true
			converters: [
				filter: (node) ->
					node.className.indexOf("md-initial-html") != -1
				replacement: (innerHTML, node) ->
					node.outerHTML
			]

		@model.get("file").set "content", toMarkdown(@mainEl.innerHTML, toMarkdownOptions)
		@model.fileIsDirty = false
		@model.saveFile()

		if @needRepaint
			@render()
			@needRepaint = false


module.exports.mimeType = /text\/markdown/

module.exports.title = rcms.tools.WebGUI.getStr "WYSIWYG"
