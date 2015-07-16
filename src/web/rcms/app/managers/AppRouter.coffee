body = document.getElementsByTagName("body")[0]

rcms.AppRouter = Backbone.Router.extend
	routes:
		"": "lsfiles"
		"lsfiles": "lsfiles"
		"editfile/*file": "editfile"

	lsfiles: ->
		@reset()
		rcms.FilesListManager.init body
		return

	editfile: (file) ->
		@reset()
		rcms.EditorManager.init body
		return

	reset: ->
		body.innerHtml = ""
		return
