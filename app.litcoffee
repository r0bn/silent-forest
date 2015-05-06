        express = require('express')
        app = express()
        http = require('http').Server(app)
        io = require('socket.io')(http)

        app.set('view engine', 'jade')
        app.set('views', './server/views')

        app.use(express.static('static'))

        app.get '/', (req, res) ->
            res.render "index"

        io.on 'connection', (socket) ->
            console.log('a userr connected')

            socket.on 'move', (msg) ->
                io.emit('move', msg)
                console.log('message: ' + msg)

        http.listen 5005, () ->
            console.log('listening on *:5000')
