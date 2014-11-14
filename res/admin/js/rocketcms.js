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
	// elements
	var fileList = $('.file-list');
	var editorArea = $('.editor-area');
	var editorResult = $('.editor-result');
	var saveBtn = $('.save-btn');
	var ajaxResult = $('.ajax-result');
	var createFile = $('.create-file');
	var createFileInput = $('.create-file-input input');
	var deleteBtn = $('.delete-btn');
	var fileUpload = $('#file-upload');

	// suggestions index
	var suggestions = new Array();
	if (fileList.length) {
		var i = 0;
		fileList.find('a').each(function() {
			var text = $(this).html();
			if (suggestions.indexOf(text) === -1) {
				suggestions[i++] = text.replace('.html', '').replace('.tt2', '');
			}
		});
		fileList.find('td').each(function() {
			var text = $(this).html();
			if (text.indexOf('▼ ') === 0) {
				text = text.substr(2);
			} else {
				return;
			}
			if (suggestions.indexOf(text) === -1) {
				suggestions[i++] = text;
			}
		});
	}

	// events
	saveBtn.click(function() {
		if (ajaxResult.html() == 'Saving...') return;
		ajaxResult.html('Saving...');
		var saveLoc = '';
		var isFile = false;
		var fileData = false;
		if (location.pathname.indexOf('/pages-edit') == 0) {
			saveLoc = '/pages-save';
		} else if (location.pathname.indexOf('/template-edit') == 0) {
			saveLoc = '/template-save';
		} else if (location.pathname.indexOf('/files-edit') == 0) {
			saveLoc = '/files-save';
			isFile = true;
			fileData = new FormData();
			fileData.append('file', fileUpload[0].files[0]);
		}
		$.ajax({
			url: saveLoc + '?file=' + getUrlParameter('file'),
			type: 'POST',
			timeout: 10000,
			dataType: 'text',
			cache: false,
			processData: false,
			data: isFile ? fileData : editorArea.html(),
			contentType: isFile ? 'multipart/form-data' : 'text/plain',
			error: function(jqXHR, text, error) {
				ajaxResult.html('<b>Connection error ' + new Date().toTimeString() + '</b>');
			},
			success: function(result) {
				ajaxResult.html(result + ' ' + new Date().toTimeString());
				if (isFile) location.reload();
			}
		});
	});

	if (editorResult.length) {
		var updateEditor = function() {
			var text = marked(htmlDecode(editorArea.html()));
			text = text.replace(/\[%/g, '<div class="intext-block">').replace(/%\]/g, '</div>');
			editorResult.html(text);
		};
		updateEditor();
		editorArea.on('input', updateEditor);
	}

	createFile.click(function() {
		if (createFileInput.parent().css("display") == "none") {
			createFileInput.parent().fadeIn();
			createFileInput.focus();
			return;
		}
		$(createFileInput).focus();

		var name = $(createFileInput).val();
		if (name == null || name == '') {
			return;
		}

		var editLoc = '';
		if (location.pathname.indexOf('/pages-list') == 0) {
			editLoc = '/pages-edit';
			name = (name + '.html').replace(/(\.html)+/g, '.html');
		} else if (location.pathname.indexOf('/template-list') == 0) {
			editLoc = '/template-edit';
			name = (name + '.tt2').replace(/(\.tt2)+/g, '.tt2');
		} else if (location.pathname.indexOf('/files-list') == 0) {
			editLoc = '/files-edit';
		}
		location.href = editLoc + '?file=' + name.replace(/\//g, '%2f');
	});

	var wasDel = false;
	createFileInput.keypress(function(e) {
		wasDel = e.keyCode == 8 || e.keyCode == 46;
		if (e.keyCode == 13) {
			createFile.click();
		}
	});
	createFileInput.on('input propertychange', function() {
		if (wasDel) return;

		var val = $(createFileInput).val();
		if (val == null || val == '') return;

		var domElem = createFileInput[0];
		if (domElem.selection) return;

		var suggestion = '';
		var start = -1;
		var suggestStart = -1;
		for (var i = 0; i < suggestions.length; i++) {
			var item = suggestions[i];
			for (var j = item.length; j > 0; j--) {
				var s = val.lastIndexOf(item.substr(0, j));
				if (s !== -1) {
					if (suggestion.length == 0 || s + j > start) {
						suggestion = item;
						suggestStart = j;
						start = s + j;
					}
				}
			}
		}
		if (start >= val.length) {
			createFileInput.val(val + suggestion.substr(suggestStart));
			domElem.selectionStart = start;
			domElem.selectionEnd = val.length + suggestion.length;
		}
	});

	deleteBtn.click(function(e) {
		e.preventDefault();
		if (confirm('Are you sure you want to delete?')) {
			location.href = $(this).attr('href');
		}
	});
});

function getUrlParameter(param) {
	var pageUrl = location.search.substring(1);
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
