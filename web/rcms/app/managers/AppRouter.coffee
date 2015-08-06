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
FileModel = require "../models/FileModel.coffee"
FilesListView = require "../views/FilesListView.coffee"
EditorModel = require "../models/EditorModel.coffee"
EditorManagerView = require "../views/EditorManagerView.coffee"

module.exports = Backbone.Router.extend
	routes:
		"": "index"
		"fileslist": "fileslist"
		"editfile/*path": "editfile"

	index: ->
		location.hash = "#fileslist"

	fileslist: ->
		rootDir = new FileModel
			path: "/"
			isDir: true
		new FilesListView
			model: rootDir
			el: @resetAndGetNewRoot()

	editfile: (path) ->
		fileModel = new FileModel
			path: path
		editorManagerModel = new EditorModel
			file: fileModel
		new EditorManagerView
			model: editorManagerModel
			el: @resetAndGetNewRoot()

	resetAndGetNewRoot: ->
		document.body.innerHTML = ""
		el = document.createElement "div"
		document.body.appendChild el
		el
