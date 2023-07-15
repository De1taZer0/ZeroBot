#include "BotInstance.hh"

namespace ZeroBot::Bot
{
    BotInstance::BotInstance()
    {
        json settings;
        std::tie( authorization, gateway, compress ) = ZeroBot::Setting::initSetting();

        HttpRequestPtr req(new HttpRequest);

        req->method = HTTP_GET;
        req->url = gateway + "?compress=" + std::to_string(compress);
        req->headers["Authorization"] = authorization;

        HttpResponsePtr resp(new HttpResponse);

        http_client_send(req.get(), resp.get());

        if(resp->GetJson().at("code").get<int>() == 0)
        {
            wsCli.onopen = []()
            {
                std::cout << "onOpen" << std::endl;
            };
            wsCli.onclose = []()
            {
                std::cout << "onClose" << std::endl;
            };
            wsCli.onmessage = [this](const std::string& msg)
            {
                std::cout << "onMessage:" + msg << std::endl;
                json msgJson(msg);
                s = msgJson.at("s");
                int code = msgJson.at("d").at("code");

                switch(s)
                {
                    case 0:
                        sn = msgJson.at("sn");
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
            wsCli.setReconnect(&reconn);
            wsCli.setPingInterval(30000);

            http_headers headers;
            headers["Authorization"] = authorization;

            wsCli.open(resp->GetJson().at("data").at("url").get<string>().c_str(), headers);

            
        }
        
    }

    BotInstance::~BotInstance()
    {

    }

} // ZeroBot