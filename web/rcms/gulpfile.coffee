gulp = require "gulp"
plugins = require("gulp-load-plugins")()
browserify = require "browserify"
source = require "vinyl-source-stream"

srcCss = [ "blocks/**/*.styl", "global/global.styl" ]
srcJs = "app/main.coffee"
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
	browserify srcJs
		.bundle()
		.pipe source "app.js"
		.pipe plugins.streamify plugins.addSrc.prepend "global/global.js"
		#.pipe plugins.streamify plugins.uglify()
		.pipe plugins.streamify plugins.concat "app.js"
		.pipe gulp.dest dst

gulp.task "data", ->
	gulp.src srcData
		.pipe gulp.dest dst

# main tasks
gulp.task "default", [ "css", "js", "data" ]
