#Name Copyright 2015 Oleg Morozenkov
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
PlainTextFileEditorView = require "./PlainTextFileEditorView.coffee"
UploadFileEditorView = require "./UploadFileEditorView.coffee"
PluginManager = require "../managers/PluginManager.coffee"

module.exports = Backbone.View.extend
	backButtonEl: null
	titleEl: null
	descEl: null
	tabsEl: null
	publishButtonEl: null
	editorEl: null

	initialize: ->
		WebGUI.getFile "templates/editor.html"
			.then (data) =>
				@initMarkup data
			.then =>
				@model.get("file").getMimeType()
			.then (mimeType) =>
				editors = @collectEditors mimeType
				isFirst = true
				for editorType in editors
					tabEl = @initTab editorType, @tabsEl, @editorEl
					if isFirst
						@setEditorActive editorType, @editorEl, tabEl, @tabsEl
						isFirst = false
			.done()

	events:
		"click .top-bar__buttons__button--publish": (e) ->
			@model.publishFile()

	initMarkup: (html) ->
		@el.innerHTML = html
		document.title = @model.get("file").get "path"
		rcms.ui.update()
		@backButtonEl = @el.getElementsByClassName("top-bar__back-button")[0]
		@titleEl = @el.getElementsByClassName("top-bar__title")[0]
		@descEl = @el.getElementsByClassName("top-bar__desc")[0]
		@tabsEl = @el.getElementsByClassName("top-bar__tabs")[0]
		@publishEl = @el.getElementsByClassName("top-bar__buttons__button--publish")[0]
		@editorEl = @el.getElementsByClassName("editor-main")[0]

		@listenTo @model, "change:fileStatus", @updateStatus

		@titleEl.innerHTML = @model.get("file").get "path"
		@backButtonEl.addEventListener "click", ->
			location.hash = "#fileslist"

	collectEditors: (mimeType) ->
		editors = [ UploadFileEditorView ]
		if PlainTextFileEditorView.mimeType.test mimeType
			editors.push PlainTextFileEditorView
		for plugin in PluginManager.plugins
			editorType = plugin.get "fileEditorViewType"
			if not editorType.mimeType? or editorType.mimeType.test mimeType
				editors.push editorType
		editors.reverse()
		editors

	initTab: (editorType, tabsDomRoot, editorDomRoot) ->
		el = document.createElement "div"
		el.classList.add "top-bar__tabs__tab"
		el.innerHTML = editorType.title
		el.addEventListener "click", (e) =>
			@setEditorActive editorType, editorDomRoot, el, tabsDomRoot
		tabsDomRoot.appendChild el
		el

	setEditorActive: (editorType, editorDomRoot, tabEl, tabsDomRoot) ->
		editorDomRoot.innerHTML = ""
		el = document.createElement "div"
		editorDomRoot.appendChild el
		new editorType
			model: @model
			el: el

		for item in tabsDomRoot.getElementsByClassName "top-bar__tabs__tab"
			item.classList.remove "top-bar__tabs__tab--active"
		tabEl.classList.add "top-bar__tabs__tab--active"

	updateStatus: ->
		@descEl.innerHTML = @model.get("fileStatus")
