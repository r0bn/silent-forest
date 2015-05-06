
    class Player

        @players = []
        @radioWidth = 150

        constructor : (startPosition, @team) ->
            @moveVector = startPosition
            @moveSpeed = 4
            @selected = false

            @pulseFrequency = 10
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

            @item = new Path.Circle(startPosition, 10)
            @item.fillColor = "black"
            @status = "grey"


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
                #@item.fillColor = "blue"
                @item2.fillColor = "blue"
                @status = "blue"
            else if red > blue
                #@item.fillColor = "red"
                @item2.fillColor = "red"
                @status = "red"
            else
                @item.fillColor = "black"
                @item2.fillColor = "grey"
                @status = "grey"

    class Referee

        constructor : () ->

            @blueCounter = 1
            @redCounter = 1

            @itemRed = new Path.Rectangle(new Point(50,10), new Size(5,20))
            @itemRed.fillColor = "red"
            @itemBlue = new Path.Rectangle(new Point(50,30), new Size(5,20))
            @itemBlue.fillColor = "blue"

        draw : () ->
            blue = 0
            red = 0
            for h in Hill.hills
                if h.status is "blue"
                    blue++
                if h.status is "red"
                    red++

            if blue > red
                @blueCounter++
                @itemBlue.scale(1.01, 1, new Point(50,10))
            else if red > blue
                @redCounter++
                @itemRed.scale(1.01, 1, new Point(50,10))

    playerPTeam = 3
    totalHills = 3
    areaRadius = 300

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
        players.push(new Player(new Point(playerRedX, playerY), "red"))
        players.push(new Player(new Point(playerBlueX, playerY), "blue"))



    ref = new Referee()


    tool = new Tool()
    tool.onMouseDown = (event) ->
        for p in players
            if p.selected
                p.moveVector = event.point

    view.onFrame = (event) ->
        for p in players
            p.draw(event)
        for h in hills
            h.draw(event)

        ref.draw(event)

