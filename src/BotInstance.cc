#include "BotInstance.hh"

namespace ZeroBot::Bot
{
    BotInstance::BotInstance()
    {
        pongMark = false;
        helloMark = false;

        reconnectMark = false;
        timeoutMark = false;
        resumeMark = false;

        resumeIntervalCnt = 0;
        resumeCnt = 1;

        std::tie( authorization, gatewayAPI, compress ) = ZeroBot::Setting::initSetting();

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

            signalQueue.emplace(std::move(SignalBase::construct(std::make_unique<json>(msg))));
        };

        pingLoopThread = std::make_unique<hv::EventLoopThread>();

        auto pingPongStat = [this](std::function<void(void)> pongFail) -> void
        {
            this->wsCli.send(PingSignal::rawString(maxSn));
            pingLoopThread->loop()->setTimeout(TIMEOUT, [this, &pongFail](auto timerID)
            {
                if(!pongMark)
                {
                    pongFail();
                }
                else
                {
                    pongMark = false;
                    timeoutMark = false;
                }
            });
        };

        pingLoopThread->loop()->setInterval(PING_INTERVAL, [this, &pingPongStat](auto timerID)
        {
            if(!timeoutMark)
            {
                pingPongStat([this, &pingPongStat]()
                {
                    timeoutMark = true;
                    timeoutLoopThread->loop()->setInterval(PING_INTERVAL << 1, [this, &pingPongStat](auto timerID)
                    {
                        pingPongStat([this, &pingPongStat]()
                        {
                            timeoutLoopThread->loop()->setInterval(PING_INTERVAL << 2, [this, &pingPongStat](auto timerID)
                            {
                                pingPongStat([this]()
                                {
                                    resumeMark = true;
                                    resumeLoopThread->start();
                                });
                            });
                        });
                    });
                });
            }
        });

        startLoopThread->loop()->setTimeout(START_TIMEOUT, [this](auto timerID)
        {
            if(!helloMark)
            {
                reconnectMark = true;
            }
        });

        auto resumeStat = [this]() -> void
        {

        };

        resumeLoopThread->loop()->setTimeout(TIMEOUT << 3, [this](auto timerID)
        {

        });
    }

    BotInstance::~BotInstance()
    {
        wsCli.close();

        pingLoopThread->loop()->stop();
    }

    auto BotInstance::getGatewayUrl() -> int
    {
        HttpRequestPtr req(new HttpRequest);

        req->method = HTTP_GET;
        req->url = gatewayAPI + "?compress=" + std::to_string(compress);
        req->headers["Authorization"] = authorization;

        HttpResponsePtr resp(new HttpResponse);

        http_client_send(req.get(), resp.get());

        websocketUrl = resp->GetJson().at("data").at("url").get<string>();

        return resp->GetJson().at("code").get<int>();
    }

    void BotInstance::run()
    {
        int res;
        while((res = getGatewayUrl()) == 0)
        {
            reconnectMark = false;

            startLoopThread->start();

            pingLoopThread->start();

            http_headers headers;
            headers["Authorization"] = authorization;

            wsCli.open(websocketUrl.c_str(), headers);

            while(!reconnectMark)
            {
                if(!signalQueue.empty())
                {
                    auto signal(std::move(signalQueue.front()));
                    signalQueue.pop();
                    switch(signal->getType())
                    {
                        case Sign::EVENT:


                        case Sign::HELLO:
                            helloMark = true;
                            break;
                        case Sign::PING:
                            break;
                        case Sign::PONG:
                            pongMark = true;
                            break;
                        case Sign::RESUME:
                            break;
                        case Sign::RECONNECT: //
                        {
                            pingLoopThread->stop(true);
                            startLoopThread->stop(true);
                            timeoutLoopThread->stop(true);
                            resumeLoopThread->stop(true);
                            maxSn = 0;
                            while(!signalQueue.empty())
                            {
                                signalQueue.pop();
                            }
                            reconnectMark = true;
                        }
                        case Sign::RESUME_ACK:
                            break;
                    }
                }
            }
        }
        if(res != 0)
        {
            std::cout << std::format("Cannot get Gateway.code:{}",res);
        }
    }

} // ZeroBot