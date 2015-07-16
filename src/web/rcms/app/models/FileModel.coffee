rcms.FileModel = Backbone.Model.extend
	defaults:
		path: null
		isDir: null
		contents: null

	create: ->
		rcms.Net.get "/api/fs/create?file=#{ @path }"

	rm: ->
		rcms.Net.get "/api/fs/rm?file=#{ @path }"

	mv: (to) ->
		rcms.Net.get "/api/fs/mv?from=#{ @path }&to=#{ to }"
			.then =>
				@path = to

	publish: ->
		rcms.Net.get "/api/fs/publish?file=#{ @path }"

	getMeta: (key) ->
		rcms.Net.get "/api/fs/getMeta?file=#{ @path }&key=#{ key }"

	setMeta: (key, value) ->
		rcms.Net.get "/api/fs/create?#{ @path }&key=#{ key }&value=#{ value }"

	checkIfIsDir: ->
		getMeta "isDir"
			.then (value) =>
				@isDir = value == "1"

	getMimeType: ->
		getMeta "mimeType"

	load: ->
		rcms.Net.get if isDir then "/api/fs/ls?dir=#{ @path }" else "/api/fs/getfile?file=#{ @path }"
			.then (data) =>
				@contents = data

	unload: ->
		@contents = null

	upload: ->
		rcms.Net.post "/api/fs/upload?#{ @path }", @contents if not isDir

	edit: ->
		# TODO