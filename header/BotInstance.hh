#ifndef ZEROBOT_BOTINSTANCE_HH
#define ZEROBOT_BOTINSTANCE_HH

///
/// @file BotInstance.hh
/// @date 2023.7.15
///

#include <queue>
#include <atomic>
#include <string>
#include <functional>
#include <hv/json.hpp>
#include <hv/requests.h>
#include <hv/EventLoop.h>
#include <hv/HttpClient.h>
#include <hv/WebSocketClient.h>
#include "Event.hh"
#include "Signal.hh"
#include "Setting.hh"

#define SN_MAX (65535)
#define PING_INTERVAL (30000)
#define TIMEOUT (6000)
#define START_TIMEOUT (6000)
#define REC_INTERVAL (1000)
#define MAX_CNT_GET_GATEWAY (60)

namespace ZeroBot::Bot
{
    using string = std::string;

    template<typename T>
    using queue = std::queue<T>;

    template<typename T>
    using priority_queue = std::priority_queue<T>;

    template<typename T>
    using unique_ptr = std::unique_ptr<T>;

    using json = nlohmann::json;

    using EventBase = Event::EventBase;
    using Event_Type = Event::Event_Type;
    using Channel_Type = Event::Channel_Type;
    using Msg_Type = Event::Msg_Type;

    using Sign = Signal::Sign;
    using SignalBase = Signal::SignalBase;
    using PingSignal = Signal::PingSignal;
    using ReconnectSignal = Signal::ReconnectSignal;

    using CallbackFuncType = std::function<void(const EventBase&)>;

    class BotInstance
    {
    private:

        std::atomic<bool> pongMark;
        std::atomic<bool> closeMark;
        std::atomic<bool> helloMark;
        std::atomic<bool> resumeMark;
        std::atomic<bool> timeoutMark;
        std::atomic<bool> gatewayMark;
        std::atomic<bool> reconnectMark;

        int compress{};
        string gatewayAPI;
        string websocketUrl;
        string authorization;
        string session_id;

        std::shared_ptr<hv::WebSocketClient> wsCli;

        hv::EventLoopThreadPtr pingLoopThread;
        hv::EventLoopThreadPtr startLoopThread;
        hv::EventLoopThreadPtr timeoutLoopThread;
        hv::EventLoopThreadPtr resumeLoopThread;

        queue<unique_ptr<SignalBase>> signalQueue;

        priority_queue<unique_ptr<EventBase>> eventQueue;

        std::unordered_map<Event::Channel_Type, CallbackFuncType> onEventFuncMap;

        [[nodiscard]] auto getGatewayUrl() -> bool;

    public:
        BotInstance();

        ~BotInstance();

        template<class EventType> auto onEvent(std::function<void(const EventType&)> callbackFunc) -> void;

        void run();

    };

    namespace _export
    {
        void func();
    }

} // ZeroBot

#endif //ZEROBOT_BOTINSTANCE_HH
