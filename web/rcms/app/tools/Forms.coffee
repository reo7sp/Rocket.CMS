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

Q = require "q"

module.exports = rcms.types.tools.Forms =
	readFile: (file, isBinary = false) ->
		Q.Promise (resolve, reject, notify) ->
			reader = new FileReader()

			reader.onload = (e) ->
				resolve e.target.result if e.target.readyState == FileReader.DONE

			reader.onerror = (e) ->
				reject new Error switch e.target.error.code
					when e.target.error.NOT_FOUND_ERR
						"File hasn't found"
					when e.target.error.NOT_READABLE_ERR
						"File isn't readable"
					when e.target.error.ABORT_ERR
						"Operation is aborted"
					else
						"An error occurred reading this file"

			reader.progress = (e) ->
				notify e.loaded / e.total if e.lengthComputable

			if isBinary
				reader.readAsBinaryString file
			else
				reader.readAsText file
