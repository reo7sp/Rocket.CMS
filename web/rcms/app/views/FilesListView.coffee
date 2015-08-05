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
	initialize: ->
		@render()

	render: ->
		WebGUI.getFile "templates/lsfiles.html"
			.then (data) =>
				@initMarkup data
			.then =>
				@model.get("dir").load() if not @model.get("dir").has "content"
			.then =>
				@insertAllFileEntries @el.getElementsByClassName("block-list")[0]
			.done()

	initMarkup: (html) ->
		@el.innerHTML = html
		document.title = WebGUI.getStr "Files list"
		rcms.ui.update()

	insertAllFileEntries: (domRoot) ->
		haveInsertedOne = false
		for path in @model.get("dir").get("content").split "\r\n"
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
