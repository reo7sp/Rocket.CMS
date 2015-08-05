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
PlainTextFileEditorView = require "./PlainTextFileEditorView.coffee"
UploadFileEditorView = require "./UploadFileEditorView.coffee"
PluginManager = require "../managers/PluginManager.coffee"

module.exports = Backbone.View.extend
	initialize: ->
		WebGUI.getFile "templates/editor.html"
			.then (data) =>
				@initMarkup data
			.then =>
				@model.get("file").getMimeType()
			.then (mimeType) =>
				editors = @collectEditors mimeType
				editorMain = @el.getElementsByClassName("editor-main")[0]
				tabsEl = @el.getElementsByClassName("top-bar__tabs")[0]
				isFirst = true
				for editorType in editors
					tabEl = @initTab editorType, tabsEl, editorMain
					if isFirst
						@setEditorActive editorType, editorMain, tabEl, tabsEl
						isFirst = false
			.done()

	initMarkup: (html) ->
		@el.innerHTML = html
		rcms.ui.update()
		document.title = @model.get("file").get "path"
		@el.getElementsByClassName("top-bar__title")[0].innerHTML = @model.get("file").get "path"
		@el.getElementsByClassName("top-bar__back-button")[0].addEventListener "click", ->
			location.hash = "#lsfiles"

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
			model: @model.get "file"
			el: el

		for item in tabsDomRoot.getElementsByClassName "top-bar__tabs__tab"
			item.classList.remove "top-bar__tabs__tab--active"
		tabEl.classList.add "top-bar__tabs__tab--active"

