/*
 * Copyright 2014 Reo_SP
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

$(document).ready(function() {
	var editorArea = $('.editor-area');
	var editorResult = $('.editor-result');
	var saveBtn = $('#save-btn');
	var ajaxResult = $('.ajax-result');
	var createFile = $('#create-file');

	saveBtn.click(function() {
		ajaxResult.html('Saving...');
		var saveLoc = '';
		if (window.location.pathname.indexOf('/pages-edit') == 0) {
			saveLoc = '/pages-save';
		} else if (window.location.pathname.indexOf('/template-edit') == 0) {
			saveLoc = '/template-save';
		}
		$.ajax({
			url: saveLoc + '?file=' + getUrlParameter('file'),
			type: 'POST',
			data: editorArea.html(),
			dataType: 'text',
			timeout: 10000,
			error: function(jqXHR, text, error) {
				ajaxResult.html('<b>Connection error ' + new Date().toTimeString() + '</b>');
			},
			success: function(result) {
				ajaxResult.html(result + ' ' + new Date().toTimeString());
			}
		});
	});

	if (editorResult.length) {
		var updateEditor = function() {
			var text = htmlDecode(editorArea.html());
			text = marked(text).replace(/\n/g, '<br>');
			editorResult.html(text);
		};
		updateEditor();
		editorArea.on('input', updateEditor);
	}

	createFile.click(function() {
		var name = prompt("Enter file name without extension");
		if (name == null || name == '') {
			return;
		}
		var editLoc = '';
		if (window.location.pathname.indexOf('/pages-list') == 0) {
			editLoc = '/pages-edit';
		} else if (window.location.pathname.indexOf('/template-list') == 0) {
			editLoc = '/template-edit';
		}
		name = '%2f' + name + '.tt';
		name = name.replace(/(\.tt)+/g, '.tt');
		name = name.replace(/(%2f)+/g, '%2f');
		window.location.href = editLoc + '?file=' + name;
	});
});

function getUrlParameter(param) {
	var pageUrl = window.location.search.substring(1);
	var args = pageUrl.split('&');
	for (var i = 0; i < args.length; i++) {
		var argName = args[i].split('=');
		if (argName[0] == param) {
			return argName[1];
		}
	}
}

function htmlEncode(text) {
	var result = text;
	result = result.replace('&', '&amp;');
	result = result.replace(' ', '&nbsp;');
	result = result.replace('<', '&lt;');
	result = result.replace('>', '&gt;');
	result = result.replace('\n', '<br>');
	return result;
}

function htmlDecode(text) {
	var result = text;
	result = result.replace(/<br>/g, '\n');
	result = result.replace(/<[^>]*>/g, '');
	result = result.replace(/&nbsp;/g, ' ');
	result = result.replace(/&lt;/g, '<');
	result = result.replace(/&gt;/g, '>');
	result = result.replace(/&amp;/g, '&');
	return result;
}
