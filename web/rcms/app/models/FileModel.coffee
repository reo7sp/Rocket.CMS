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
Q = require "q"
Net = require "../tools/Net.coffee"

module.exports = rcms.types.models.FileModel = Backbone.Model.extend
	defaults: ->
		path: null
		isDir: null
		metaCache: {}
		content: null

	create: ->
		@set "isDir", false if not @has "isDir"
		Net.get "/api/fs/create?#{ if @get "isDir" then "dir" else "file" }=#{ @get "path" }"

	rm: ->
		Net.get "/api/fs/rm?file=#{ @get "path" }"

	mv: (to) ->
		Net.get "/api/fs/mv?from=#{ @get "path" }&to=#{ to }"
			.then =>
				@set "path", to
				to

	publish: ->
		Net.get "/api/fs/publish?file=#{ @get "path" }"

	getMeta: (key) ->
		metaCache = @get("metaCache")
		result = metaCache[key]
		if result?
			if result.then?
				result
			else
				Q.fcall ->
					result
		else
			p = Net.get "/api/fs/getmeta?file=#{ @get "path" }&key=#{ key }"
			metaCache[key] = p
			p.then (value) =>
				metaCache[key] = value
			p.fail (err) ->
				metaCache[key] = null
				throw err

	setMeta: (key, value) ->
		Net.get "/api/fs/create?file=#{ @get "path" }&key=#{ key }&value=#{ value }"
			.then (data) =>
				@get("metaCache")[key] = value
				data

	checkIfIsDir: ->
		@getMeta "_isDir"
			.then (value) =>
				@set "isDir", value == "1"
				value

	getMimeType: ->
		@getMeta "_mimeType"

	load: ->
		Net.get if @get "isDir" then "/api/fs/ls?dir=#{ @get "path" }" else "/api/fs/getfile?file=#{ @get "path" }"
			.then (data) =>
				@set "content", data
				data

	unload: ->
		@set "content", null

	upload: ->
		Net.post "/api/fs/upload?file=#{ @get "path" }", @get "content" if not @get "isDir"
