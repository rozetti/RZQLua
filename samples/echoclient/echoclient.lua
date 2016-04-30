rz.import("debug")
rz.import("QWebSocket")
rz.import("QUrl")

u = QUrl("ws://localhost:8080")

ws = QWebSocket()

ws.connected = function()
    debug("connected in lua")
    ws.sendTextMessage("hello me")
end

ws.disconnected = function()
    debug("disconnected in Lua")
end

ws.textMessageReceived = function(message)
    debug("received in lua: "..message)
    ws.close(QWebSocket.CloseCodeNormal, "closed after message received")
end

ws.open(u)

debug("loaded echoclient.lua")
