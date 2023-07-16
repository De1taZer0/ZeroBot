#include "BotInstance.hh"

namespace ZeroBot::Bot
{
    BotInstance::BotInstance()
    {

        closeMark = false;

        pingLoopThread = std::make_unique<hv::EventLoopThread>();
        startLoopThread = std::make_unique<hv::EventLoopThread>();
        timeoutLoopThread = std::make_unique<hv::EventLoopThread>();
        resumeLoopThread = std::make_unique<hv::EventLoopThread>();

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

            try
            {
                signalQueue.emplace(std::move(SignalBase::construct(std::move(json::parse(msg)))));
            }
            catch(std::exception& e)
            {
                std::cout << e.what() << std::endl;
            }
        };

        auto pingPongStat = [this](std::function<void(void)> pongFailFunc) -> void
        {
            this->wsCli.send(PingSignal::rawString(EventBase::getMaxSN()));
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
            this->wsCli.send(ReconnectSignal::rawString(EventBase::getMaxSN()));
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
                        reconnectMark = true;
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

    template<class EventType>
    auto BotInstance::onEvent(std::function<void(const EventType&)> callbackFunc) -> void
    {
        EventType event;
        onEventFuncMap[event.getType()] = [callbackFunc](const EventBase& msg) -> void
        {
            try
            {
                callbackFunc(static_cast<const EventType&>(msg));
            }
            catch(const std::exception& e)
            {
                const auto& eventType = msg.getType();
                std::cerr << eventType.cType << " " << eventType.mType << std::endl;
                std::cerr << "\tBotInstance::onEvent error: " << e.what() << std::endl;
            }
        };
    }

    auto BotInstance::run() -> void
    {
        while(!closeMark)
        {
            gatewayMark = getGatewayUrl();
            int getGatewayInterval = REC_INTERVAL;
            if(!gatewayMark)
            {
                hv_msleep(getGatewayInterval);
                std::cout << "Connect fail, trying to re-get gateway." << std::endl;
                getGatewayInterval = min(REC_INTERVAL * MAX_CNT_GET_GATEWAY, getGatewayInterval << 1);
            }
            pongMark = false;
            helloMark = false;
            resumeMark = false;
            timeoutMark = false;
            reconnectMark = false;

            http_headers headers;
            headers["Authorization"] = authorization;

            wsCli.open(websocketUrl.c_str(), headers);

            hv_msleep(500);

            startLoopThread->start();
            pingLoopThread->start();

            while(!reconnectMark)
            {
                if(!signalQueue.empty())
                {
                    auto signal = std::move(signalQueue.front());
                    signalQueue.pop();
                    switch(signal->getType())
                    {
                        case Sign::EVENT:
                            eventQueue.emplace(std::move(EventBase::construct(signal->getRawMsg().get<json>())));
                            break;
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
                            EventBase::resetMaxSN();
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
                if(!eventQueue.empty())
                {
                    std::shared_ptr<EventBase> event = std::move(const_cast<unique_ptr<EventBase>&>(eventQueue.top()));
                    eventQueue.pop();
                    if(onEventFuncMap.count(event->getType()) != 0)
                    {
                        onEventFuncMap.at(event->getType())(*event);
                    }
                }
            }

            wsCli.close();
        }
    }

    void _export::func()
    {
        BotInstance bot;
        bot.onEvent<Event::EventGroupMsg>([](auto&){});
        bot.onEvent<Event::EventPersonMsg>([](auto&){});
        bot.onEvent<Event::EventBroadcastMsg>([](auto&){});
    }

} // ZeroBot