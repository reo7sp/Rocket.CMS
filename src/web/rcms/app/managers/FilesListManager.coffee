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