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

        auto pingPongStat = [this](std::function<void(void)> pongFailFunc) -> void
        {
            this->wsCli.send(PingSignal::rawString(maxSn));
            pingLoopThread->loop()->setTimeout(TIMEOUT, [this, &pongFailFunc](auto timerID)
            {
                if(!pongMark)
                {
                    pongFailFunc();
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
                    timeoutLoopThread->loop()->setTimeout(REC_INTERVAL << 1, [this, &pingPongStat](auto timerID)
                    {
                        pingPongStat([this, &pingPongStat]()
                        {
                            timeoutLoopThread->loop()->setTimeout(REC_INTERVAL << 2, [this, &pingPongStat](auto timerID)
                            {
                                pingPongStat([this]()
                                {
                                    resumeMark = true;
                                    std::cout << "Timeout, resuming." << std::endl;
                                    if(getGatewayUrl())
                                    {
                                        resumeLoopThread->start();
                                    }
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

        auto resumeStat = [this](std::function<void(void)> resumeFailFunc) -> void
        {
            this->wsCli.send(ReconnectSignal::rawString(maxSn));
            resumeLoopThread->loop()->setTimeout(TIMEOUT, [this, &resumeFailFunc](auto timerID)
            {
                if(resumeMark)
                {
                    resumeFailFunc();
                }
                else
                {
                    timeoutMark = false;
                }
            });
        };

        resumeLoopThread->loop()->runInLoop([this, &resumeStat]()
        {
            resumeStat([this, &resumeStat]()
            {
                resumeLoopThread->loop()->setTimeout(REC_INTERVAL << 3, [this, &resumeStat](auto timerID)
                {
                    resumeStat([this]()
                    {
                        int getGatewayInterval = REC_INTERVAL;
                        while(getGatewayUrl())
                        {
                            hv_msleep(getGatewayInterval);
                            std::cout << "Resume fail, trying to get gateway." << std::endl;
                            getGatewayInterval = min(REC_INTERVAL * MAX_CNT_GET_GATEWAY, getGatewayInterval << 1);
                        }
                        timeoutMark = false;
                        resumeMark = false;
                    });
                });
            });
        });

    }

    BotInstance::~BotInstance()
    {
        wsCli.close();

        pingLoopThread->loop()->stop();
    }

    auto BotInstance::getGatewayUrl() -> bool
    {
        HttpRequestPtr req(new HttpRequest);

        req->method = HTTP_GET;
        req->url = gatewayAPI + "?compress=" + std::to_string(compress);
        req->headers["Authorization"] = authorization;

        HttpResponsePtr resp(new HttpResponse);

        http_client_send(req.get(), resp.get());

        websocketUrl = resp->GetJson().at("data").at("url").get<string>();

        auto code = resp->GetJson().at("code").get<int>();
        if(code == 0)
        {
           return true;
        }
        else
        {
            std::cout << std::format("Cannot get Gateway.code:{}", code);
            return false;
        }
    }

    void BotInstance::run()
    {
        while(getGatewayUrl())
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
                        case Sign::RECONNECT:
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
                            resumeMark = false;
                            break;
                    }
                }
            }
        }
    }

} // ZeroBot