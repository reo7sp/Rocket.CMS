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
