gulp = require "gulp"
plugins = require("gulp-load-plugins")()
browserify = require "browserify"
source = require "vinyl-source-stream"
buffer = require "vinyl-buffer"
globby = require "globby"
through = require "through2"

srcCss = [ "blocks/**/*.styl", "global/global.styl" ]
srcJs = [ "app/main.coffee", "blocks/**/*.coffee" ]
srcData = [ "**/*.html", "!bin/**", "!node_modules/**" ]
dst = "bin"

# source tasks
gulp.task "css", ->
	gulp.src srcCss
		.pipe plugins.concat "style.styl"
		.pipe plugins.stylus()
		.pipe plugins.autoprefixer()
		.pipe plugins.concat "style.css"
		.pipe gulp.dest dst

gulp.task "js", ->
	s = through()
	s
		.pipe source "app.js"
		.pipe buffer()
		.pipe plugins.addSrc.prepend "global/global.js"
		#.pipe plugins.uglify()
		.pipe plugins.concat "app.js"
		.pipe gulp.dest dst

	globby srcJs, (err, files) ->
		if err
			bundledStream.emit "error", err
			return
		browserify(files).bundle().pipe s

	s

gulp.task "data", ->
	gulp.src srcData
		.pipe gulp.dest dst

# main tasks
gulp.task "default", [ "css", "js", "data" ]
