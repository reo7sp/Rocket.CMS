rcms.Net =
	request: (method, path, data) ->
		Q.Promise (resolve, reject, notify) ->
			handle = new XMLHttpRequest()

			handle.onload = ->
				if handle.status == 200
					resolve handle.response
				else
					reject new Error "Status code is #{ handle.status }"

			handle.onerror = ->
				reject new Error "Can't establish connection"

			handle.onprogress = (e) ->
				notify e.loaded / e.total

			handle.open method, path, true
			handle.send data

			return

	get: (path) ->
		@request "GET", path, null

	post: (path, data) ->
		@request "POST", path, data