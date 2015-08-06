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
FileModel = require "../models/FileModel.coffee"

module.exports = Backbone.View.extend
	isChildrenFolded: true
	arrowEl: null
	titleEl: null
	descEl: null
	childrenRoot: null
	newfileButtonEl: null
	openInTabButtonEl: null

	initialize: ->
		WebGUI.getFile "templates/lsfilesItem.html"
			.then (data) =>
				@initMarkup data
				@render()
			.done()

	events: ->
		"click .file-entry__clickable-space": (e) =>
			if not @model.has "isDir"
				return
			if @model.get "isDir"
				@isChildrenFolded = !@isChildrenFolded
				if @model.has "content"
					@render()
				else
					@model.load()
						.then (data) =>
							@insertAllChildren @childrenRoot
							@render()
						.done()
			else
				@openEditor false

		"mousedown .file-entry__clickable-space": (e) =>
			if e.which != 2
				return
			if @model.has("isDir") and not @model.get "isDir"
				@openEditor true

		"click .file-entry__buttons__button--onenintab": (e) =>
			@openEditor true

		"click .file-entry__buttons__button--newfile": (e) =>
			return # TODO

		"click .file-entry__buttons__button--move": (e) =>
			return # TODO

		"click .file-entry__buttons__button--delete": (e) =>
			return # TODO

	render: ->
		@renderArrow()
		@renderButtons()
		@renderTitle()
		rcms.ui.update()

	initMarkup: (html) ->
		@el.innerHTML = html
		@arrowEl = @el.getElementsByClassName("file-entry__arrow")[0]
		@titleEl = @el.getElementsByClassName("file-entry__title")[0]
		@descEl = @el.getElementsByClassName("file-entry__desc")[0]
		@childrenRoot = @el.getElementsByClassName("file-entry__children")[0]
		@newfileButtonEl = @el.getElementsByClassName("file-entry__buttons__button--newfile")[0]
		@openInTabButtonEl = @el.getElementsByClassName("file-entry__buttons__button--openintab")[0]

	renderArrow: ->
		if not @model.has "isDir"
			@model.checkIfIsDir()
				.then (value) =>
					@render()
				.done()
		else if @model.get "isDir"
			if @isChildrenFolded
				@arrowEl.classList.add "file-entry__arrow--folded"
				@arrowEl.classList.remove "file-entry__arrow--unfolded"
				@childrenRoot.classList.add "file-entry__children--folded"
			else
				@arrowEl.classList.add "file-entry__arrow--unfolded"
				@arrowEl.classList.remove "file-entry__arrow--folded"
				@childrenRoot.classList.remove "file-entry__children--folded"

	renderButtons: ->
		if @model.has "isDir"
			if @model.get "isDir"
				@newfileButtonEl.classList.remove "file-entry__buttons__button--hidden"
			else
				@openInTabButtonEl.classList.remove "file-entry__buttons__button--hidden"

	renderTitle: ->
		pathParts = @model.get("path").split "/"
		@titleEl.innerHTML = pathParts[pathParts.length - 1]
		if @model.has "metaTitleFail"
		else if not @model.has "metaTitle"
			@model.getMeta "title"
				.then (value) =>
					if value
						@model.set "metaTitle", value
					else
						@model.set "metaTitleFail", true
					@render()
				.fail =>
					@model.set "metaTitleFail", true
					@render()
				.done()
		else
			@titleEl.innerHTML = @model.get "metaTitle"
			@descEl.innerHTML = pathParts[pathParts.length - 1]

	insertAllChildren: (domRoot) ->
		for path in @model.get("content").split "\r\n"
			if not path
				continue
			model = new FileModel
				path: @model.get("path") + "/" + path
			@insertChild model, domRoot
			haveInsertedOne = true
		return

	insertChild: (model, domRoot) ->
		el = document.createElement "div"
		domRoot.appendChild el
		new module.exports
			model: model
			el: el

	openEditor: (isInNewTab = false) ->
		if isInNewTab
			w = window.open "#editfile/#{ @model.get "path" }", "_blank"
			w.focus()
		else
			location.hash = "#editfile/#{ @model.get "path" }"
