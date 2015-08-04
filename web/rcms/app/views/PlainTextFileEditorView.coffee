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
PlainTextFileEditorView = require "../views/PlainTextFileEditorView.coffee"

module.exports = Backbone.View.extend
	initialize: ->
		@model.on "change:content", @render, @
		@el.contentEditable = true

	events:
		'input': ->
			@model.set "content", @el.innerHTML

	render: ->
		@el.innerHTML = @model.content

module.exports.mimeType = /(text|application)\//

PlainTextFileEditorView.title = WebGUI.getStr "plain_text_editor"
