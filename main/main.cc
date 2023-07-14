#include <iostream>
#include <hv/requests.h>
#include <hv/WebSocketClient.h>
#include "Setting.hh"

std::pair<int, std::string> getWSUrl(const std::string& gateWayUrl,const int& compress, const std::string& auth);

int main()
{
    using nlohmann::json;

    SetConsoleOutputCP(65001);

    json settings;
    ZeroBot::Setting::initSetting(settings);

    std::string authorization = settings["Authorization"];

    auto res = getWSUrl(settings["Gateway"], 0, authorization);

    if(res.first == 0)
    {
        hv::WebSocketClient ws;
        ws.onopen = [&ws]()
        {
            std::cout << "onOpen" << std::endl;
        };
        ws.onclose = [&ws]()
        {
            std::cout << "onClose" << std::endl;
        };
        ws.onmessage = [](const std::string& msg)
        {
            std::cout << "onMessage:" + msg << std::endl;
        };

        hv::ReconnectInfo reconn;
        ws.setReconnect(&reconn);
        ws.setPingInterval(30000);

        http_headers headers;
        headers["Authorization"] = authorization;

        ws.open(res.second.c_str(), headers);

        std::cout << ws.isConnected() << std::endl;

        while(ws.isConnected())
        {

        }
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