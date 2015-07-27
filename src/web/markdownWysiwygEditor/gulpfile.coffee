gulp      = require "gulp"
plugin    = require("gulp-load-plugins")()

srcCss    = "lib/**/*.css"
srcAppJs  = "app/**/*.coffee"
srcLibJs  = "lib/**/*.js"
srcData   = [ "**/*.@(eot|svg|ttf|woff)", "!bin/**", "!node_modules/**" ]
dst       = "bin"

# source tasks
gulp.task "css", ->
	gulp.src srcCss
		.pipe plugin.stylus()
		.pipe plugin.concat "style.css"
		.pipe gulp.dest dst

gulp.task "js", ->
	gulp.src srcAppJs
		.pipe plugin.coffee()
		.pipe plugin.addSrc srcLibJs
		.pipe plugin.uglify()
		.pipe plugin.concat "app.js"
		.pipe gulp.dest dst

gulp.task "data", ->
	gulp.src srcData
		.pipe gulp.dest dst

# main tasks
gulp.task "default", [ "css", "js" ]
