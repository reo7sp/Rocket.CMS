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
Forms = require "../tools/Forms.coffee"
WebGUI = require "../tools/WebGUI.coffee"

module.exports = Backbone.View.extend
	statusEl: null
	fileEl: null

	initialize: ->
		WebGUI.getFile "templates/upload-file-editor.html"
			.then (data) =>
				@initMarkup data
			.done()

	initMarkup: (html) ->
		@el.innerHTML = html
		rcms.ui.update()
		@statusEl = @el.getElementsByClassName("upload-file-editor__status")[0]
		@fileEl = @el.getElementsByClassName("upload-file-editor__file")[0]

		@model.get("file").getMimeType()
			.then (mimeType) =>
				@fileEl.accept = mimeType
			.done()

	events:
		"change .upload-file-editor__file": (e) ->
			@model.set "fileIsDirty", true
			Forms.readFile e.target.files[0], true
				.then (data) =>
					@model.get("file").set "content", data
					@model.set "fileIsDirty", false
					@model.saveFile()
				.fail (err) =>
					@statusEl.innerHTML = WebGUI.getStr err.message
					@model.set "fileIsDirty", false

module.exports.title = WebGUI.getStr "Upload"
