gulp         = require "gulp"
plug         = require("gulp-load-plugins")()

srcCss       = [ "blocks/**/*.styl", "lib/**/*.css", "global/global.styl" ]
srcAppJs     = "app/**/*.coffee"
srcLibJs     = "lib/**/*.js"
srcImg       = "img/**/*.@(png|jpg|gif|webm|ico)"
srcData      = [ "**/*.@(html|txt|ico)", "!bin/**", "!node_modules/**" ]
dst          = "bin"

# source tasks
gulp.task "css", ->
	gulp.src srcCss
		.pipe plug.plumber()
		.pipe plug.cached "css"
		.pipe plug.stylus()
		.pipe plug.autoprefixer()
		.pipe plug.remember "css"
		.pipe plug.concat "style.css"
		.pipe gulp.dest dst
		.pipe plug.livereload()

gulp.task "appjs", ->
	gulp.src srcAppJs
		.pipe plug.plumber()
		.pipe plug.cached "appjs"
		.pipe plug.coffee()
		.pipe plug.remember "appjs"
		.pipe plug.addSrc.prepend "global/global.js"
		.pipe plug.concat "app.js"
		.pipe gulp.dest dst
		.pipe plug.livereload()

gulp.task "libjs", ->
	gulp.src srcLibJs
		.pipe plug.concat "lib.js"
		.pipe gulp.dest dst
		.pipe plug.livereload()

gulp.task "img", ->
	gulp.src srcImg
		.pipe plug.plumber()
		.pipe plug.cached "img"
		.pipe gulp.dest "#{ dst }/img"
		.pipe plug.livereload()
		.pipe plug.remember "img"

gulp.task "data", ->
	gulp.src srcData
		.pipe plug.plumber()
		.pipe plug.cached "data"
		.pipe gulp.dest dst
		.pipe plug.livereload()
		.pipe plug.remember "data"

# utility tasks
gulp.task "watch", ->
	plug.livereload.listen()
	gulp.watch srcCss, [ "css" ]
	gulp.watch srcAppJs, [ "appjs" ]
	gulp.watch srcLibJs, [ "libjs" ]
	gulp.watch srcImg, [ "img" ]
	gulp.watch srcData, [ "data" ]

# main tasks
gulp.task "build", [ "css", "appjs", "libjs", "img", "data" ]

gulp.task "default", [ "build", "watch" ]
