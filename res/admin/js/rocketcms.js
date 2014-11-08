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
	$('#save-btn').click(function() {
		var saveLoc = '';
		if (window.location.pathname.indexOf('/pages-edit') == 0) {
			saveLoc = '/pages-save';
		} else if (window.location.pathname.indexOf('/template-edit') == 0) {
			saveLoc = '/template-save';
		}
		var timeout;
		$.ajax({
			url: saveLoc + '?file=' + getUrlParameter('file'),
			type: 'POST',
			data: $('.editor-area').html() + "###END###",
			success: function(result) {
				clearTimeout(timeout);
				$('.ajax-result').html(result);
				timeout = setTimeout(function() {
					$('.ajax-result').html('');
				}, 2500);
			}
		});
	});
	if ($('.editor-result').length) {
		var syncResult = function() {
			var text = htmlDecode($('.editor-area').html());
			text = marked(text).replace(/\n/g, '<br>');
			$('.editor-result').html(text);
		};
		syncResult();
		$('.editor-area').on('input', syncResult);
	}
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

function htmlDecode(text, decodeTags) {
	var result = text;
	result = result.replace(/<br>/g, '\n');
	result = result.replace(/<[^>]*>/g, '');
	result = result.replace(/&nbsp;/g, ' ');
	result = result.replace(/&lt;/g, '<');
	result = result.replace(/&gt;/g, '>');
	result = result.replace(/&amp;/g, '&');
	return result;
}
