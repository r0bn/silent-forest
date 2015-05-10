
    class Player

        @players = []
        @radioWidth = 150

        constructor : (startPosition, @team, @id) ->
            @moveVector = startPosition
            @moveSpeed = 4
            @selected = false

            @pulseFrequency = 2
            @pulseLast = 0

            @item = new Path.Circle(startPosition, 20)
            @item.fillColor = @team

            @selector = new Path.Circle(startPosition, 22)
            @selector.strokeColor = @team
            @selector.opacity = 0


            @item.onMouseDown = (event) =>
                if !@selected
                    for p in Player.players
                        p.selected = false
                        p.selector.opacity = 0
                        p.item.fillColor = p.team
                @selected = !@selected
                if @selected
                    @selector.opacity = 0.8
                else
                    @selector.opacity = 0
        
        draw : (event) =>
            vector = @moveVector - @item.position
            if vector.length > 3
                @item.position += vector.normalize() * @moveSpeed * (event.delta / 0.0166)

            @selector.position = @item.position

            if (event.time - @pulseLast) > @pulseFrequency
                if @pulse?
                    @pulse.remove()
                @pulse = new Path.Circle(@item.position,20)
                @pulse.strokeColor = "black"

                @pulse.opacity = 0.3
                @pulseLast = event.time
            else
                if @pulse?
                    if @pulse.bounds.width < 300
                        @pulse.scale 1.06
                        @pulse.position = @item.position
                    else
                        @pulse.remove()

    class Hill

        @hills = []

        constructor : (startPosition) ->

            @item2 = new Path.Circle(startPosition, 150)
            @item2.fillColor = "grey"
            @item2.opacity = 0.5

            @item = new Path.Circle(startPosition, 20)
            @item.fillColor = "black"
            @status = "grey"

            @conquerStatus = new Path.Circle(startPosition, 25)
            @conquerStatus.strokeColor = "black"
            @conquerStatus.strokeWidth = 5

            @conquerTime = 0.0
            #min @timetoConquer = 0.1
            @timeToConquer = 3 


        draw : (event) ->
            blue = 0
            red = 0
            for p in Player.players
                if (p.item.position - @item.position).length < Player.radioWidth
                    if p.team is "blue"
                        blue++
                    if p.team is "red"
                        red++


            if blue > red
                @conquerTime -= event.delta
                if Math.abs(@conquerTime) > @timeToConquer
                    @conquerTime = -1 * @timeToConquer
            else if red > blue
                @conquerTime += event.delta
                if Math.abs(@conquerTime) > @timeToConquer
                    @conquerTime = @timeToConquer

            if @conquerStatus?
                @conquerStatus.remove()


            if Math.abs(@conquerTime) != @timeToConquer
                @conquerStatus = new Path.Arc(getArcCirclePos(@item.position, 25, @conquerTime/@timeToConquer))
            else
                @conquerStatus = new Path.Circle(@item.position, 25)
            @conquerStatus.strokeWidth = 5
            @conquerStatus.strokeColor = "black"

            if @conquerTime <= (-1 * @timeToConquer)
                @conquerTime = (-1 * @timeToConquer)
                @item2.fillColor = "blue"
                @conquerStatus.strokeColor = "blue"
                @status = "blue"
            else if @conquerTime >= @timeToConquer
                @conquerTime = @timeToConquer
                @item2.fillColor = "red"
                @conquerStatus.strokeColor = "red"
                @status = "red"
            else if @conquerTime < 0 and @status == "red" or @conquerTime > 0 and @status == "blue"
                @item2.fillColor = "grey"
                @conquerStatus.strokeColor = "grey"
                @status = "grey"
            

    class Referee

        constructor : () ->

            @blueCounter = 1
            @redCounter = 1

            @itemsRed = []
            @itemsBlue = []

            @lastCount = 0
            @countFrequency = 3


            blueOrder = new Path.Rectangle(new Point(0,25), new Size(602,14))
            blueOrder.strokeColor = "blue"

            redOrder = new Path.Rectangle(new Point(0,8), new Size(602,14))
            redOrder.strokeColor = "red"

        draw : (event) ->

            if @blueCounter >= 51 or @redCounter >= 51
                return

            if (event.time - @lastCount) > @countFrequency
                blue = 0
                red = 0
                for h in Hill.hills
                    if h.status is "blue"
                        blue++
                    if h.status is "red"
                        red++

                if blue > red
                    for i in [1..(blue - red)]
                        if @blueCounter >= 51
                            break
                        itemBlue = new Path.Rectangle(new Point(2+(@itemsBlue.length * 12),27), new Size(10,10))
                        itemBlue.fillColor = "blue"
                        @itemsBlue.push itemBlue
                        @blueCounter++
                else if red > blue
                    for i in [1..(red - blue)]
                        if @redCounter >= 51
                            break
                        itemRed = new Path.Rectangle(new Point(2+(@itemsRed.length * 12),10), new Size(10,10))
                        itemRed.fillColor = "red"
                        @itemsRed.push itemRed
                        @redCounter++
                @lastCount = event.time

    socket = io()
    socket.on 'move', (msg) ->
        console.log  new Point(msg.targetPoint[1], msg.targetPoint[2])
        for p in Player.players
            if p.id is msg.sourceID
                p.moveVector = new Point(msg.targetPoint[1], msg.targetPoint[2])


    playerPTeam = 5
    totalHills = 4
    areaRadius = 250

    areaCenterX = 50 + 20 + Player.radioWidth + areaRadius
    areaCenterY = 50 + 20 + Player.radioWidth + areaRadius
    areaCenter = new Point(areaCenterX,areaCenterY)

    getHillPos = (pos) ->
        posDelta = 2 * Math.PI / totalHills
        hillX = areaCenter.x + areaRadius * Math.cos((pos - 1) * posDelta - Math.PI/2)
        hillY = areaCenter.y + areaRadius * Math.sin((pos - 1) * posDelta - Math.PI/2)
        new Point(hillX, hillY)


    hills = Hill.hills
    for pos in [1..totalHills]
        hills.push(new Hill(getHillPos(pos)))


    players = Player.players
    for num in [1..playerPTeam]
        playerY = areaCenter.y - (playerPTeam - 1)*50/2 + (num - 1) * 50
        playerRedX = 50
        playerBlueX = areaCenter.x + (areaCenter.x - 50)
        players.push(new Player(new Point(playerRedX, playerY), "red", num))
        players.push(new Player(new Point(playerBlueX, playerY), "blue", num + 100))

    getArcCirclePos = (point, radius, percent) ->
        #percent = 0.001 if percent < 0.001
        from = new Point()
        through = new Point()
        to = new Point()
        
        from.x = point.x
        from.y = point.y - radius
        
        through.x = point.x + radius * Math.cos(2 * Math.PI * percent/2 - Math.PI/2)
        through.y = point.y + radius * Math.sin(2 * Math.PI * percent/2 - Math.PI/2)
        
        to.x = point.x + radius * Math.cos(2 * Math.PI * percent - Math.PI/2)
        to.y = point.y + radius * Math.sin(2 * Math.PI * percent - Math.PI/2)

        return {from, through, to}


    ref = new Referee()


    tool = new Tool()
    tool.onMouseDown = (event) ->
        for p in players
            if p.selected
                p.moveVector = event.point
                socket.emit('move', {
                    targetPoint : event.point
                    sourceID : p.id
                })

    view.onFrame = (event) ->
        for p in players
            p.draw(event)
        for h in hills
            h.draw(event)

        ref.draw(event)

