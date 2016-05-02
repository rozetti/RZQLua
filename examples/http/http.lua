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
    local s = reply2.readAll()

    debug("finished finished: "..s)
    debug("reply: "..reply.instanceName)
    debug("reply2: "..reply2.instanceName)
end

reply2 = m.get(r)
debug("reply2: "..reply2.instanceName)

debug("http.lua loaded")
