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

rcms.FilesListManager =
	init: (domRoot) ->
		rcms.WebGUI.getFile "templates/lsfiles.html"
			.then (value) ->
				domRoot.innerHtml = value

			.then ->
				rcms.WebGUI.getStr "files_list"
					.then (value) ->
						domRoot.getElementsByClassName("top-menu__title")[0].innerHtml = value

			.then ->
				rootDir = new rcms.FileModel
					path: "/"
					isDir: true

				rootDir.load
					.then (data) ->
						childrenDomRoot = domRoot.getElementsByClassName("block-list")[0]
						for childPath in data.split '\n'
							el = childrenDomRoot.createElement "div"
							childrenDomRoot.appendChild el

							childModel = new rcms.FileModel
								path: childPath

							childView = new rcms.FileListEntryView
								model: childModel
								el
						return

			.done()