    gulp =            require 'gulp'
    gutil =           require 'gulp-util'

    # server
    browserSync =     require 'browser-sync'
    reload =          browserSync.reload
    server =          require "gulp-develop-server"

    # js
    coffee =          require 'gulp-coffee'
    uglify =          require 'gulp-uglify'
    concat =          require 'gulp-concat'

    # html
    jade =            require 'gulp-jade'

    # css 
    stylus =          require 'gulp-stylus'

    # documentation
    globby =          require 'globby'
    exec =            require('child_process').exec

# File Creation

## HTML

    gulp.task 'html', () ->
      gulp.src './assets/jade/*.jade'
      .pipe jade()
      .pipe gulp.dest './static'
      .pipe reload({stream:true})

## JS

    gulp.task 'js', () ->
      gulp.src './assets/coffee/**/*.litcoffee'
      .pipe coffee({bare : true}).on('error', gutil.log)
      .pipe concat 'main.js'
      .pipe gulp.dest './static/build'
      .pipe reload({stream:true})

    gulp.task 'js-vendor', () ->
        gulp.src [
            './bower_components/paper/dist/paper-full.js'
        ]
        .pipe concat 'vendor.js'
        #.pipe uglify()
        .pipe gulp.dest './static/build'
        .pipe reload({stream:true})

## CSS 

    gulp.task 'css', () ->
      gulp.src './assets/stylus/**/*.styl'
      .pipe stylus()
      .pipe gulp.dest './static/build'
      .pipe reload({stream:true})

    gulp.task 'css-vendor', () ->
        ###
        gulp.src [
        ]
        .pipe concat 'vendor.css'
        .pipe gulp.dest './static/build'
        .pipe reload({stream:true})
        ###

## Documentation

    gulp.task 'docs', () ->
        globby ["README.md", "assets/**/*", "gulpfile.litcoffee"], (err, files) ->
            fileList = files.join(" ")

            exec __dirname + "/node_modules/docco/bin/docco -o docs/ -L " +__dirname + "/docs/language.json -l linear " + fileList, (err, stdout, stderr) ->
                console.log stdout
                console.log stderr

## Build all

    gulp.task 'build', ['js', 'css', 'html', 'js-vendor', 'css-vendor'], () ->

## Deployment

# Watch

    gulp.task 'watch', ['build'], () ->
      gulp.watch ['assets/coffee/**/*'],['js']
      gulp.watch ['assets/stylus/**/*'],['css']
      gulp.watch ['assets/jade/**/*'], ['html']
      gulp.watch ['app.litcoffee', 'server/**/*'], [server.restart]

    gulp.task 'browser-sync', ['watch'], () ->
      browserSync {
        proxy : "localhost:5005"
        open: false
        port : 5000
      }

# Server

    gulp.task 'server', () ->
      server.listen { path: 'app.litcoffee' }

    gulp.task 'default', ['browser-sync', 'server']
