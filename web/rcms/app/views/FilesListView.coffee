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
FilesListEntryView = require "./FilesListEntryView.coffee"

module.exports = Backbone.View.extend
	entriesRoot: null

	initialize: ->
		WebGUI.getFile "templates/lsfiles.html"
			.then (data) =>
				@initMarkup data
			.then =>
				@model.load() if not @model.has "content"
			.then =>
				@insertAllFileEntries @entriesRoot
			.done()

	events:
		"click .top-bar__buttons__button--newfile": (e) ->
			fileName = prompt WebGUI.getStr "Enter new file's name"
			if fileName?
				newFile = new FileModel
					path: fileName
					isDir: false
				newFile.create()
				@insertFileEntry newFile, @entriesRoot if fileName.indexOf("/") != 0

		"click .top-bar__buttons__button--newfolder": (e) ->
			folderName = prompt WebGUI.getStr "Enter new folders's name"
			if folderName?
				newFolder = new FileModel
					path: folderName
					isDir: true
				newFolder.create()
				@insertFileEntry newFolder, @entriesRoot if folderName.indexOf "/" != 0

	initMarkup: (html) ->
		@el.innerHTML = html
		document.title = WebGUI.getStr "Files list"
		rcms.ui.update()
		@entriesRoot = @el.getElementsByClassName("block-list")[0]

	insertAllFileEntries: (domRoot) ->
		haveInsertedOne = false
		for path in @model.get("content").split "\r\n"
			if not path
				continue
			model = new FileModel
				path: path
			@insertFileEntry model, domRoot
			haveInsertedOne = true
		if not haveInsertedOne
			domRoot.innerHTML = "<p>#{ WebGUI.getStr "No files" }</p>"

	insertFileEntry: (model, domRoot) ->
		el = document.createElement "div"
		domRoot.appendChild el
		new FilesListEntryView
			model: model
			el: el
