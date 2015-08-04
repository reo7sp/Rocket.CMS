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
	initialize: ->
		WebGUI.getFile "templates/lsfilesItem.html"
			.then (data) =>
				@el.innerHTML = data
				@render()
			.done()

	isChildrenFolded: true

	events: ->
		"click .file-entry__title": (e) =>
			if @model.get "isDir"
				@isChildrenFolded = !@isChildrenFolded
				if @model.has "contents"
					@render()
				else
					childrenDomRoot = @el.getElementsByClassName("file-entry__children")[0]
					@model.load()
						.then (data) =>
							for childPath in data.split "\r\n"
								if not childPath
									continue
								el = document.createElement "div"
								childrenDomRoot.appendChild el

								childModel = new FileModel
									path: @model.get("path") + "/" + childPath

								childView = new module.exports
									model: childModel
									el: el
							@render()
						.done()
			else
				location.hash = "#editfile/#{ @model.get "path" }"

	render: ->
		if not @model.has "isDir"
			@model.checkIfIsDir()
				.then (value) =>
					@render()
				.done()
		else if @model.get "isDir"
			arrowEl = @el.getElementsByClassName("file-entry__arrow")[0]
			arrowEl.classList.remove "file-entry__arrow--folded"
			arrowEl.classList.remove "file-entry__arrow--unfolded"
			arrowEl.classList.add "file-entry__arrow--#{ if not @isChildrenFolded then "un" else "" }folded"

			childrenDomRoot = @el.getElementsByClassName("file-entry__children")[0]
			if @isChildrenFolded
				childrenDomRoot.classList.add "file-entry__children--folded"
			else
				childrenDomRoot.classList.remove "file-entry__children--folded"

		if @model.has "metaTitleFail"
			pathParts = @model.get("path").split "/"
			@el.getElementsByClassName("file-entry__title")[0].innerHTML = pathParts[pathParts.length - 1]
		else if not @model.has "metaTitle"
			@model.getMeta "title"
				.then (value) =>
					@model.set "metaTitle", value
					@render()
				.fail =>
					@model.set "metaTitleFail", true
				.done()
		else if @model.get "metaTitle"
			pathParts = @model.get("path").split "/"
			@el.getElementsByClassName("file-entry__title")[0].innerHTML = @model.get "metaTitle"
			@el.getElementsByClassName("file-entry__desc")[0].innerHTML = pathParts[pathParts.length - 1]


