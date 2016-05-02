rz.import("debug")
rz.import("QUrl")
rz.import("QNetworkRequest")
rz.import("QNetworkReply")
rz.import("QNetworkAccessManager")

u = QUrl("http://google.com")
r = QNetworkRequest(u)
m = QNetworkAccessManager()

m.finished = function(reply)
    debug("finished started")
    local s = reply.readAll()
    debug("finished finished: "..s)
end

m.get(r)

debug("http.lua loaded")
