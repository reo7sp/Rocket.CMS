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
		getMeta "_isDir"
			.then (value) =>
				@isDir = value == "1"

	getMimeType: ->
		getMeta "_mimeType"

	load: ->
		rcms.Net.get if isDir then "/api/fs/ls?dir=#{ @path }" else "/api/fs/getfile?file=#{ @path }"
			.then (data) =>
				@contents = data

	unload: ->
		@contents = null

	upload: ->
		rcms.Net.post "/api/fs/upload?#{ @path }", @contents if not isDir
