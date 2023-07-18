#include "BotInstance.hh"

namespace ZeroBot::Bot
{
    BotInstance::BotInstance()
    {

        closeMark = false;

        pingLoopThread = std::make_unique<hv::EventLoopThread>();
        startLoopThread = std::make_unique<hv::EventLoopThread>();
        resumeLoopThread = std::make_unique<hv::EventLoopThread>();
        timeoutLoopThread = std::make_unique<hv::EventLoopThread>();

        std::tie( authorization, gatewayAPI, compress ) = ZeroBot::Setting::initSetting();

        wsCli = std::make_shared<hv::WebSocketClient>();

        wsCli->onopen = []()
        {
            std::cout << "onOpen" << std::endl;
        };
        wsCli->onclose = []()
        {
            std::cout << "onClose" << std::endl;
        };
        wsCli->onmessage = [this](const std::string& msg)
        {
            std::cout << "onMessage:" + msg << std::endl;
            try
            {
                signalQueue.emplace(std::move(SignalBase::construct(std::move(json::parse(msg)))));
            }
            catch(std::exception& e)
            {
                std::cout << "\t--OnMessage: " << msg << std::endl;
            }
        };

        pingLoopThread->loop()->setInterval(PING_INTERVAL, [this](auto timerID) -> void
        {
            if(!timeoutMark)
            {
                std::cout << "pinging..." << std::endl;
                wsCli->send(PingSignal::rawString(EventBase::getMaxSN()));
                pingLoopThread->loop()->setTimeout(TIMEOUT, [this](auto timerID)
                {
                    if(!pongMark)
                    {
                        timeoutMark = true;
                        std::cout << "\tping timeout, trying to ping: 1" << std::endl;
                        timeoutLoopThread->loop()->setTimeout(REC_INTERVAL << 1, [this](auto timerID) -> void
                        {
                            std::cout << "pinging...1" << std::endl;
                            wsCli->send(PingSignal::rawString(EventBase::getMaxSN()));
                            pingLoopThread->loop()->setTimeout(TIMEOUT, [this](auto timerID)
                            {
                                if(!pongMark)
                                {
                                    std::cout << "\tping timeout, trying to ping: 2" << std::endl;
                                    timeoutLoopThread->loop()->setTimeout(REC_INTERVAL << 2, [this](auto timerID) -> void
                                    {
                                        std::cout << "pinging...2" << std::endl;
                                        wsCli->send(PingSignal::rawString(EventBase::getMaxSN()));
                                        pingLoopThread->loop()->setTimeout(TIMEOUT, [this](auto timerID)
                                        {
                                            if(!pongMark)
                                            {
                                                resumeMark = true;
                                                std::cout << "Timeout, resuming." << std::endl;
                                                resumeLoopThread->start();
                                            }
                                            else
                                            {
                                                pongMark = false;
                                                timeoutMark = false;
                                            }
                                        });
                                    });
                                }
                                else
                                {
                                    pongMark = false;
                                    timeoutMark = false;
                                }
                            });
                        });
                    }
                    else
                    {
                        pongMark = false;
                        timeoutMark = false;
                    }
                });
            }
        });

        startLoopThread->loop()->setTimeout(START_TIMEOUT, [this](auto timerID) -> void
        {
            if(!helloMark)
            {
                reconnectMark = true;
            }
        });

        resumeLoopThread->loop()->setTimeout(REC_INTERVAL << 3, [this](auto timerID) -> void
        {
            this->wsCli->send(ReconnectSignal::rawString(EventBase::getMaxSN()));
            resumeLoopThread->loop()->setTimeout(TIMEOUT, [this](auto timerID) -> void
            {
                if(resumeMark)
                {
                    resumeLoopThread->loop()->setTimeout(REC_INTERVAL << 4, [this](auto timerID) -> void
                    {
                        this->wsCli->send(ReconnectSignal::rawString(EventBase::getMaxSN()));
                        resumeLoopThread->loop()->setTimeout(TIMEOUT, [this](auto timerID) -> void
                        {
                            if(resumeMark)
                            {
                                reconnectMark = true;
                            }
                            else
                            {
                                timeoutMark = false;
                            }
                        });
                    });
                }
                else
                {
                    timeoutMark = false;
                }
            });
        });
    }

    BotInstance::~BotInstance()
    {
        wsCli->close();

        pingLoopThread->stop(true);
        startLoopThread->stop(true);
        resumeLoopThread->stop(true);
        timeoutLoopThread->stop(true);
    }

    auto BotInstance::getGatewayUrl() -> bool
    {
        HttpRequestPtr req(new HttpRequest);

        req->method = HTTP_GET;
        req->url = gatewayAPI + "?compress=" + std::to_string(compress);
        req->headers["Authorization"] = authorization;

        if(reconnectMark)
        {
            req->url += "&resume=1&sn=" + std::to_string(EventBase::getMaxSN()) + "&session_id=" + session_id;
        }

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
        onEventFuncMap[event.getType().cType] = [callbackFunc](const EventBase& msg) -> void
        {
            try
            {
                callbackFunc(static_cast<const EventType&>(msg));
            }
            catch(const std::exception& e)
            {
                const auto& eventType = msg.getType();
                std::cout << "Error occurred when calling function onEvent: " << eventType.cType << " " << eventType.mType;
                std::cout << "\n\t" << e.what();
                std::cout << "\n\t\t--In BotInstance::onEvent error: " << std::endl;
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

            wsCli->open(websocketUrl.c_str(), headers);

            hv_msleep(500);

            startLoopThread->start();
            pingLoopThread->start();

            while(!reconnectMark)
            {
                if(!signalQueue.empty())
                {
                    auto signal = std::move(signalQueue.front());
                    signalQueue.pop();
                    try
                    {
                        switch(signal->getType())
                        {
                            case Sign::EVENT:
                                std::cout << "--On event:" << signal->getRawMsg() << std::endl;
                                eventQueue.emplace(std::move(EventBase::construct(signal->getRawMsg().get<json>())));
                                break;
                            case Sign::HELLO:
                                std::cout << "--On hello:" << signal->getRawMsg() << std::endl;
                                session_id = signal->getRawMsg().at("d").at("session_id").get<string>();
                                helloMark = true;
                                break;
                            case Sign::PING:
                                break;
                            case Sign::PONG:
                                std::cout << "--On pong:" << signal->getRawMsg() << std::endl;
                                pongMark = true;
                                break;
                            case Sign::RESUME:
                                break;
                            case Sign::RECONNECT:
                            {
                                std::cout << "--On reconnect:" << signal->getRawMsg() << std::endl;
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
                    catch(const std::exception& e)
                    {
                        std::cout << "\t--On processing signal:" << signal->getType() << ": " << e.what() << std::endl;
                    }
                }
                if(!eventQueue.empty())
                {
                    std::shared_ptr<EventBase> event = std::move(const_cast<unique_ptr<EventBase>&>(eventQueue.top()));
                    eventQueue.pop();
                    if(onEventFuncMap.count(event->getType().cType) != 0)
                    {
                        onEventFuncMap.at(event->getType().cType)(*event);
                    }
                }
            }

            wsCli->close();

            pingLoopThread->stop(true);
            startLoopThread->stop(true);
            resumeLoopThread->stop(true);
            timeoutLoopThread->stop(true);
        }
    }

    auto BotInstance::sendGroupMsg(const int& type, const string& target_id, const string& content, const string& quote, const string& temp_target_id) -> std::pair<string, int>
    {
        HttpRequestPtr req(new HttpRequest);

        req->method = HTTP_POST;
        req->url = "https://www.kookapp.cn/api/v3/message/create";
        req->json = { { "type"          , type           },
                      { "target_id"     , target_id      },
                      { "content"       , content        },
                      { "quote"         , quote          },
                      { "temp_target_id", temp_target_id } };

        try
        {
            HttpResponsePtr resp(new HttpResponse);

            http_client_send(req.get(), resp.get());

            json res = resp->GetJson();

            if(res.at("code").get<int>() != 0)
            {
                std::cerr << "Send GroupMsg failed. Error code: " << res.at("code").get<int>();

            }

            std::cout << std::atoi(resp->GetHeader("X-Rate-Limit-Limit", "0").c_str()) << std::endl;
            std::cout << std::atoi(resp->GetHeader("X-Rate-Limit-Remaining", "0").c_str()) << std::endl;
            std::cout << std::atoi(resp->GetHeader("X-Rate-Limit-Reset", "0").c_str()) << std::endl;
            std::cout << std::atoi(resp->GetHeader("X-Rate-Limit-Bucket", "0").c_str()) << std::endl;
            std::cout << std::atoi(resp->GetHeader("X-Rate-Limit-Global", "0").c_str()) << std::endl;

            return std::make_pair(res.at("data").at("msg_id").get<string>(), res.at("data").at("msg_timestamp").get<int>());
        }
        catch(const std::exception& e)
        {
            std::cerr << "Error occurred when sending GroupMsg: " << req->body;
            std::cerr << "\n\t" << e.what();
        }
    }

    auto BotInstance::sendGroupMsg(const string &target_id, const string &content) -> std::pair<string, int>
    {
        HttpRequestPtr req(new HttpRequest);

        req->method = HTTP_POST;
        req->url = "https://www.kookapp.cn/api/v3/message/create";
        req->headers["Authorization"] = authorization;
        req->json = { { "target_id" , target_id },
                      { "content"   , content   } };

        try
        {
            HttpResponsePtr resp(new HttpResponse);

            http_client_send(req.get(), resp.get());

            json res = resp->GetJson();

            std::cout << res << std::endl;
            if(res.at("code").get<int>() != 0)
            {
                std::cout << req->body << std::endl;
                std::cerr << "Send GroupMsg failed. Error code: " << res.at("code").get<int>() << std::endl;
                return std::make_pair("" , 0);
            }

            std::cout << std::atoi(resp->GetHeader("X-Rate-Limit-Limit", "0").c_str()) << std::endl;
            std::cout << std::atoi(resp->GetHeader("X-Rate-Limit-Remaining", "0").c_str()) << std::endl;
            std::cout << std::atoi(resp->GetHeader("X-Rate-Limit-Reset", "0").c_str()) << std::endl;
            std::cout << std::atoi(resp->GetHeader("X-Rate-Limit-Bucket", "0").c_str()) << std::endl;
            std::cout << std::atoi(resp->GetHeader("X-Rate-Limit-Global", "0").c_str()) << std::endl;

            return std::make_pair(res.at("data").at("msg_id").get<string>(), res.at("data").at("msg_timestamp").get<int>());
        }
        catch(const std::exception& e)
        {
            std::cerr << "Error occurred when sending GroupMsg: " << req->body;
            std::cerr << "\n\t" << e.what();
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