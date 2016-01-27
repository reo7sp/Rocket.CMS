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

rcms.ui.updateListeners.push ->
	fixedTopBars = document.getElementsByClassName("top-bar--fixed")
	if fixedTopBars.length < 1 or document.getElementById("_fixed-toolbar-body-fix")?
		return
	document.body.innerHTML = "<div id='_fixed-toolbar-body-fix' style='position: relative; top: #{ fixedTopBars[0].offsetHeight }px'>#{ document.body.innerHTML }</div>"
