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

WebGUI = require "../tools/WebGUI.coffee"
FileModel = require "../models/FileModel.coffee"
FileListEntryView = require "../views/FileListEntryView.coffee"

module.exports =
	init: (domRoot) ->
		WebGUI.getFile "templates/lsfiles.html"
			.then (data) ->
				domRoot.innerHTML = data

			.then ->
				domRoot.getElementsByClassName("top-bar__title")[0].innerHTML = WebGUI.getStr "files_list"

			.then ->
				rootDir = new FileModel
					path: "/"
					isDir: true
				rootDir.load()

			.then (data) ->
				childrenDomRoot = domRoot.getElementsByClassName("block-list")[0]
				for childPath in data.split "\r\n"
					if not childPath
						continue
					el = document.createElement "div"
					childrenDomRoot.appendChild el

					childModel = new FileModel
						path: childPath

					childView = new FileListEntryView
						model: childModel
						el: el
				return

			.done()
