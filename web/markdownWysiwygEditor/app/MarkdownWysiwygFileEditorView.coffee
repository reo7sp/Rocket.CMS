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
WebGUI = rcms.types.tools.WebGUI

module.exports = Backbone.View.extend
	needRepaint: false
	syncModelTimeoutHandle: null
	autoSyncModelTimeoutHandle: null
	mainEl: null

	initialize: ->
		WebGUI.getFile "plugins/markdownWysiwygEditor/templates/mwfe.html"
			.then (data) =>
				@initMarkup data
				@initModel()
			.done()

	events:
		"input .mwfe__main": (e) ->
			@onInput()

		"paste .mwfe__main": (e) ->
			@needRepaint = true
			setTimeout @syncModel.bind(@), 0

		"blur .mwfe__main": (e) ->
			setTimeout @syncModel.bind(@), 0

		"click .mwfe__toolbar__item--bold": (e) ->
			@surroundRange document.createElement "strong"

		"click .mwfe__toolbar__item--italic": (e) ->
			@surroundRange document.createElement "em"

		"click .mwfe__toolbar__item--underline": (e) ->
			@surroundRange document.createElement "u"

		"click .mwfe__toolbar__item--strikethrough": (e) ->
			@surroundRange document.createElement "s"

		"click .mwfe__toolbar__item--ul": (e) ->
			el = document.createElement "ul"
			for i in [1..3]
				childEl = document.createElement "li"
				childEl.innerHTML = "#{ WebGUI.getStr "Entry" } ##{ i }"
				el.appendChild childEl
			@insertToRange el

		"click .mwfe__toolbar__item--ol": (e) ->
			el = document.createElement "ol"
			for i in [1..3]
				childEl = document.createElement "li"
				childEl.innerHTML = "#{ WebGUI.getStr "Entry" } ##{ i }"
				el.appendChild childEl
			@insertToRange el

		"click .mwfe__toolbar__item--link": (e) ->
			el = document.createElement "a"
			el.href = prompt WebGUI.getStr "Enter link"
			@surroundRange el if el.href?

		"click .mwfe__toolbar__item--image": (e) ->
			el = document.createElement "a"
			el.href = prompt WebGUI.getStr "Enter link"
			@surroundRange el if el.href?
			@surroundRange document.createElement "img"

		"click .mwfe__toolbar__item--quote": (e) ->
			@surroundRange document.createElement "blockquote"

		"click .mwfe__toolbar__item--code": (e) ->
			@surroundRange document.createElement "code"

		"click .mwfe__toolbar__item--hr": (e) ->
			@insertToRange document.createElement "hr"

		"click .mwfe__toolbar__item--table": (e) ->
			el = document.createElement "table"
			rows = prompt WebGUI.getStr "Enter number of rows"
			if not rows?
				return
			if rows < 1
				rows = 1
			columns = prompt WebGUI.getStr "Enter number of columns"
			if not columns?
				return
			if columns < 1
				columns = 1
			theadEl = document.createElement "thead"
			tbodyEl = document.createElement "tbody"
			for i in [0..rows]
				trEl = document.createElement "tr"
				for j in [1..columns]
					if i == 0
						thEl = document.createElement "th"
						thEl.innerHTML = "#{ WebGUI.getStr "Entry" } ##{ j } #{ WebGUI.getStr "in header row" }"
						trEl.appendChild thEl
					else
						tdEl = document.createElement "td"
						tdEl.innerHTML = "#{ WebGUI.getStr "Entry" } ##{ j } #{ WebGUI.getStr "in row" } ##{ i }"
						trEl.appendChild tdEl
				if i == 0
					theadEl.appendChild trEl
				else
					tbodyEl.appendChild trEl
			el.appendChild theadEl
			el.appendChild tbodyEl
			@insertToRange el

		"click .mwfe__toolbar__item--h2": (e) ->
			@surroundRange document.createElement "h2"

		"click .mwfe__toolbar__item--h3": (e) ->
			@surroundRange document.createElement "h3"

		"click .mwfe__toolbar__item--h4": (e) ->
			@surroundRange document.createElement "h4"

		"click .mwfe__toolbar__item--h5": (e) ->
			@surroundRange document.createElement "h5"

		"click .mwfe__toolbar__item--h6": (e) ->
			@surroundRange document.createElement "h6"


	render: ->
		renderer = new marked.Renderer()
		renderer.html = (html) ->
			result = html
			result = result.replace /class="/g, "class=\"_md-initial-html "
			result = result.replace /class='/g, "class='_md-initial-html "
			result = result.replace /class=([^\s]+?)/g, "class=\"_md-initial-html $1\"" if result == html
			result = result.replace /<[^/](.+?)>/g , "<$1 class=\"_md-initial-html\">" if result == html
			result
		markedOptions =
			renderer: renderer
			gfm: true
			tables: true
			breaks: true
			smartLists: true
		@mainEl.innerHTML = marked @model.get("file").get("content"), markedOptions

	initMarkup: (html) ->
		@el.innerHTML = html
		rcms.ui.update()
		@mainEl = @el.getElementsByClassName("mwfe__main")[0]

	initModel: ->
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
				{
					filter: (node) ->
						node.className.indexOf("_md-initial-html") != -1
					replacement: (innerHTML, node) ->
						node.outerHTML
							.replace /\ class="_md-initial-html"/g, ""
							.replace /\ class="_md-initial-html /g, " class=\""
							.replace /\ class='_md-initial-html /g, " class='"
				}
				{
					filter: [ 'div', 'span' ]
					replacement: (innerHTML, node) ->
						innerHTML
				}
			]
		c = @mainEl.innerHTML
			.replace /<\/strong><strong>/g, ""
			.replace /<\/em><em>/g, ""
			.replace /<\/u><u>/g, ""
			.replace /<\/s><s>/g, ""
			.replace /<\/code><code>/g, ""
			.replace /<\/h2><h2>/g, ""
			.replace /<\/h3><h3>/g, ""
			.replace /<\/h4><h4>/g, ""
			.replace /<\/h5><h5>/g, ""
			.replace /<\/h6><h6>/g, ""
		@model.get("file").set "content", toMarkdown(c, toMarkdownOptions)
		@model.set "fileIsDirty", false
		@model.saveFile()

		if @needRepaint
			@render()
			@needRepaint = false

	onInput: ->
		@model.set "fileIsDirty", true
		clearTimeout @syncModelTimeoutHandle
		@syncModelTimeoutHandle = setTimeout @syncModel.bind(@), 2500
		@autoSyncModelTimeoutHandle = setTimeout @syncModel.bind(@), 15000 if not @autoSyncModelTimeoutHandle?

	insertToRange: (el) ->
		range = window.getSelection().getRangeAt 0
		range.collapse true
		range.insertNode el
		@onInput()

	surroundRange: (el) ->
		range = window.getSelection().getRangeAt 0
		if not @mainEl.contains(range.startContainer) or not @mainEl.contains range.endContainer
			return
		el.appendChild range.extractContents()
		range.insertNode el
		@onInput()


module.exports.mimeType = /text\/markdown/

module.exports.title = WebGUI.getStr "WYSIWYG"
