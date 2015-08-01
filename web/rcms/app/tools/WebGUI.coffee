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

cacheFiles = {}
cacheStrs = {}

getSomething = (key, cache, method) ->
	result = cache[key]
	if result?
		if result.then?
			result
		else
			Q.fcall ->
				result
	else
		p = rcms.Net.get "/api/webgui/#{ method }?file=#{ key }"
		cache[key] = p
		p.then (value) ->
			cache[key] = value
		p.catch ->
			cache[key] = undefined

rcms.WebGUI =
	getFile: (key) ->
		getSomething key, cacheFiles, "getfile"

	getStr: (key) ->
		getSomething key, cacheStrs, "getstr"
