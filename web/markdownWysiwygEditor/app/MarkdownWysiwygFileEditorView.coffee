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
	initialize: ->
		@model.on "change:content", @render, @
		@el.contentEditable = true

	editorMain: null

	lastTimeoutHandle: null

	updateModelContent: ->
		toMarkdownOptions =
			gfm: true
			converters:
				filter: (node) ->
					node.className.indexOf "md-initial-html" != -1
				replacement: (innerHTML, node) ->
					node.outerHTML

		@model.set "content", toMarkdown(@el.innerHTML, toMarkdownOptions)

	events:
		'input': (e) ->
			clearTimeout lastTimeoutHandle if lastTimeoutHandle?
			lastTimeoutHandle = setTimeout updateModelContent, 3000

	render: ->
		renderer = new marked.Renderer()
		renderer.html = (html) ->
			result = html
			result = result.replace "class=\"", "class=\"md-initial-html "
			result = result.replace "class='", "class='md-initial-html "
			result = result.replace /(<.+? )/gi , "$1 class=\"md-initial-html\" " if result == html
			result

		@el.innerHTML = marked @model.content, { renderer }


module.exports.mimeType = /text\/markdown/

module.exports.title = rcms.WebGUI.getStr "wysiwyg_text_editor"
