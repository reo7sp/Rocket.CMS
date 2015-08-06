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
	clickableSpaceEl: null
	buttonsEl: null
	childrenRoot: null
	openInTabButtonEl: null
	newFileButtonEl: null
	newFolderButtonEl: null

	initialize: ->
		WebGUI.getFile "templates/lsfilesItem.html"
			.then (data) =>
				@initMarkup data
				@render()
			.done()

	events:
		"click .file-entry__clickable-space": (e) ->
			if e.target != @clickableSpaceEl and e.target.parentElement != @clickableSpaceEl
				return
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

		"mousedown .file-entry__clickable-space": (e) ->
			if e.target != @clickableSpaceEl and e.target.parentElement != @clickableSpaceEl
				return
			@openEditor true if e.which == 2 and @model.has("isDir") and not @model.get "isDir"

		"click .file-entry__buttons__button--openintab": (e) ->
			if e.target.parentElement != @buttonsEl
				return
			@openEditor true

		"click .file-entry__buttons__button--newfile": (e) ->
			if e.target.parentElement != @buttonsEl
				return
			fileName = prompt WebGUI.getStr "Enter new file's name"
			if fileName?
				newFile = new FileModel
					path: @model.get("path") + "/" + fileName
					isDir: false
				newFile.create()
				@insertChild newFile, @childrenRoot if fileName.indexOf "/" != 0

		"click .file-entry__buttons__button--newfolder": (e) ->
			if e.target.parentElement != @buttonsEl
				return
			folderName = prompt WebGUI.getStr "Enter new folder's name"
			if folderName?
				newFolder = new FileModel
					path: @model.get("path") + "/" + folderName
					isDir: true
				newFolder.create()
				@insertChild newFile, @childrenRoot if folderName.indexOf "/" != 0

		"click .file-entry__buttons__button--delete": (e) ->
			if e.target.parentElement != @buttonsEl
				return
			if confirm WebGUI.getStr "Are your sure you want to permanently delete this?"
				@model.rm()
				@model.unload()
				@el.parentElement.removeChild @el

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
		@clickableSpaceEl = @el.getElementsByClassName("file-entry__clickable-space")[0]
		@buttonsEl = @el.getElementsByClassName("file-entry__buttons")[0]
		@childrenRoot = @el.getElementsByClassName("file-entry__children")[0]
		@openInTabButtonEl = @el.getElementsByClassName("file-entry__buttons__button--openintab")[0]
		@newFileButtonEl = @el.getElementsByClassName("file-entry__buttons__button--newfile")[0]
		@newFolderButtonEl = @el.getElementsByClassName("file-entry__buttons__button--newfolder")[0]

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
				@newFileButtonEl.classList.remove "file-entry__buttons__button--hidden"
				@newFolderButtonEl.classList.remove "file-entry__buttons__button--hidden"
			else
				@openInTabButtonEl.classList.remove "file-entry__buttons__button--hidden"

	renderTitle: ->
		pathParts = @model.get("path").split "/"
		@titleEl.innerHTML = pathParts[pathParts.length - 1]
		if @model.get("isDir") or @model.has "metaTitleFail"
			return
		if @model.has "metaTitle"
			@titleEl.innerHTML = @model.get "metaTitle"
			@descEl.innerHTML = pathParts[pathParts.length - 1]
		else
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
