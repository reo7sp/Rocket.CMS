gulp      = require "gulp"
plugin    = require("gulp-load-plugins")()

srcAppCss = "blocks/**/*.styl"
srcLibCss = "lib/**/*.css"
srcAppJs  = "app/**/*.coffee"
srcLibJs  = "lib/**/*.js"
srcData   = [ "**/*.html", "!bin/**", "!node_modules/**" ]
dst       = "bin"

# source tasks
gulp.task "css", ->
	gulp.src srcAppCss
		.pipe plugin.stylus()
		.pipe plugin.autoprefixer()
		.pipe plugin.addSrc srcLibCss
		.pipe plugin.concat "style.css"
		.pipe gulp.dest dst

gulp.task "js", ->
	gulp.src srcAppJs
		.pipe plugin.coffee()
		.pipe plugin.addSrc srcLibJs
		.pipe plugin.addSrc.prepend "global/global.js"
		.pipe plugin.uglify()
		.pipe plugin.concat "app.js"
		.pipe gulp.dest dst

gulp.task "data", ->
	gulp.src srcData
		.pipe gulp.dest dst

# main tasks
gulp.task "default", [ "css", "js", "data" ]
