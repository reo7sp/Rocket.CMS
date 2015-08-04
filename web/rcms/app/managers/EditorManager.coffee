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
PlainTextFileEditorView = require "../views/PlainTextFileEditorView.coffee"
PluginManager = require "../managers/PluginManager.coffee"

module.exports =
	init: (domRoot, model) ->
		WebGUI.getFile "templates/editor.html"
			.then (data) ->
				domRoot.innerHTML = data

			.then ->
				domRoot.getElementsByClassName("top-bar__title")[0].innerHTML = model.path

			.then ->
				model.getMimeType

			.then (mimeType) ->
				editors = []
				if PlainTextFileEditorView.mimeType.test mimeType
					editor.push PlainTextFileEditorView
				for plugin in PluginManager.plugins
					E = plugin.get "fileEditorViewType"
					if not E.mimeType? or E.mimeType.test mimeType
						editors.push E

				tabsEl = domRoot.getElementsByClassName("top-bar__tabs")[0]
				for editorType in editors
					tabEl = document.createElement "div"
					tabEl.className = "top-bar__tabs__tab"
					tabEl.innerHTML = editorType.title
					tabEl.onclick = do (tabEl, editorType) ->
						for tab1El in tabsEl.getElementsByClassName "top-bar__tabs__tab"
							tab1El.className = tab1El.className.replace " top-bar__tabs__tab--active", ""
						tabEl.className += " top-bar__tabs__tab--active"

						editorMain = domRoot.getElementsByClassName("editor-main")[0]
						editorMain.innerHTML = ""
						el = document.createElement "div"
						el.appendChild editorMain
						editor = new editorType
							model: model
							el: el
					tabsEl.appendChild tabEl

			.done()
