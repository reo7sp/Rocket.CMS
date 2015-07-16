rcms.PlainTextFileEditorView = Backbone.View.extend
	initialize: ->
		@model.on "change:content", @render, @
		@el.contentEditable = true

	events:
		'onchange': ->
			@model.set "content", @el.innerHtml

	render: ->
		@el.innerHtml = @model.content

rcms.PlainTextFileEditorView.mimeType = /(text|application)\//
rcms.PlainTextFileEditorView.title = rcms.WebGUI.getStr "plain_text_editor"