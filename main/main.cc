#include <iostream>
#include <hv/requests.h>
#include <hv/WebSocketClient.h>
#include <hv/json.hpp>
#include "BotInstance.hh"

#define MAXSN

std::pair<int, std::string> getWSUrl(const std::string& gateWayUrl,const int& compress, const std::string& auth);

int main()
{
    using nlohmann::json;

    logger_set_level(hlog, LOG_LEVEL_DEBUG);

    SetConsoleOutputCP(65001);

    json settings;
    ZeroBot::Setting::initSetting(settings);

    std::string authorization = settings["Authorization"];

    auto res = getWSUrl(settings["Gateway"], 0, authorization);

    if(res.first == 0)
    {
        int s, snMax = 0, sn;
        hv::WebSocketClient ws;
        ws.onopen = []()
        {
            std::cout << "onOpen" << std::endl;
        };
        ws.onclose = []()
        {
            std::cout << "onClose" << std::endl;
        };
        ws.onmessage = [&s,&sn](const std::string& msg)
        {
            std::cout << "onMessage:" + msg << std::endl;
            json msgJson(msg);
            s = msgJson["s"];
            int code = msgJson["d"]["code"];

            switch(s)
            {
                case 0:
                    sn = msgJson["sn"];

                    break;
                case 1:
                    break;
                case 3:
                    break;
                case 5:
                    break;
                case 6:
                    break;
            }
        };

        hv::ReconnectInfo reconn;
        ws.setReconnect(&reconn);
        ws.setPingInterval(30000);

        http_headers headers;
        headers["Authorization"] = authorization;

        ws.open(res.second.c_str(), headers);

        while(ws.loop()->isRunning());
    }

    return 0;
}

std::pair<int, std::string> getWSUrl(const std::string& gateWayUrl,const int& compress, const std::string& auth)
{
    std::string param = "?compress=" + std::to_string(compress);

    HttpRequestPtr req(new HttpRequest);

    req->method = HTTP_GET;
    req->url = gateWayUrl + param;
    req->headers["Authorization"] = auth;

    HttpResponsePtr resp(new HttpResponse);

    http_client_send(req.get(), resp.get());

    return std::make_pair(resp->GetJson()["code"], resp->GetJson()["data"]["url"]);
}