
    class Player

        @players = []

        constructor : (startPosition, @team) ->
            @moveVector = startPosition
            @moveSpeed = 4
            @selected = false
            @radioWidth = 150

            @pulseFrequency = 2
            @pulseLast = 0

            @item = new Path.Circle(startPosition, 20)
            @item.fillColor = @team


            @item.onMouseDown = (event) =>
                if !@selected
                    for p in Player.players
                        p.selected = false
                        p.item.fillColor = p.team
                @selected = !@selected
                if @selected
                    @item.fillColor = "green"
                else
                    @item.fillColor = @team
        
        draw : (event) =>
            vector = @moveVector - @item.position
            if vector.length > 3
                @item.position += vector.normalize() * @moveSpeed * (event.delta / 0.0166)

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
            @item.fillColor = "grey"
            @status = "grey"


        draw : (event) ->
            blue = 0
            red = 0
            for p in Player.players
                if (p.item.position - @item.position).length < p.radioWidth
                    if p.team is "blue"
                        blue++
                    if p.team is "red"
                        red++


            if blue > red
                @item.fillColor = "blue"
                @status = "blue"
            else if red > blue
                @item.fillColor = "red"
                @status = "red"
            else
                @item.fillColor = "grey"
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




    hills = Hill.hills
    hills.push(new Hill(new Point(500,500)))
    hills.push(new Hill(new Point(300,800)))


    players = Player.players
    players.push(new Player(new Point(100,100),"red"))
    players.push(new Player(new Point(150,150), "red"))
    players.push(new Player(new Point(200,200), "blue"))
    players.push(new Player(new Point(250,250), "blue"))


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

