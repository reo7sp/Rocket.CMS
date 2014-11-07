$(document).ready(function() {
	$('#save-btn').click(function() {
		var saveLoc = '';
		if (window.location.pathname.indexOf('/pages-edit') == 0) {
			saveLoc = '/pages-save';
		} else if (window.location.pathname.indexOf('/template-edit') == 0) {
			saveLoc = '/template-save';
		}
		console.log('html:' + $('.editor-area').html());
		var timeout;
		$.ajax({
			url: saveLoc + '?file=' + getUrlParameter('file'),
			type: 'POST',
			//data: $('.editor-area').html(),
			data: "hey, yay",
			success: function(result) {
				clearTimeout(timeout);
				$('.ajax-result').html(result);
				timeout = setTimeout(function() {
					$('.ajax-result').html('');
				}, 2500);
			}
		});
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
