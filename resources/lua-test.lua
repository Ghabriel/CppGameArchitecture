player = {
    pos = {
        X = 20,
        Y = 30,
    },
    filename = "loremipsum.png",
    HP = 20,
-- you can also have comments
}

function sum(x, y)
    return x + y
end

function setPlayerHP(value)
    player.HP = value
end

function testCppFunction()
    write("Hello, world!")
    local x = 42
    write("Answer: "..x)
end
