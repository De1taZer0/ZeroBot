#ifndef ZEROBOT_BOTINSTANCE_HH
#define ZEROBOT_BOTINSTANCE_HH

///
/// @file BotInstance.hh
/// @date 2023.7.15
///

#include <queue>
#include <string>
#include <functional>
#include <hv/json.hpp>
#include <hv/requests.h>
#include <hv/EventLoop.h>
#include <hv/WebSocketClient.h>
#include "Setting.hh"
#include "Signal.hh"

#define SN_MAX (65535)
#define PING_INTERVAL (30000)
#define TIMEOUT (6000)
#define START_TIMEOUT (6000)

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

    using Sign = Signal::Sign;
    using SignalBase = Signal::SignalBase;
    using PingSignal = Signal::PingSignal;

    class BotInstance
    {
    private:
        hv::WebSocketClient wsCli;

        hv::EventLoopThreadPtr pingLoopThread;
        hv::EventLoopThreadPtr startLoopThread;
        hv::EventLoopThreadPtr timeoutLoopThread;
        hv::EventLoopThreadPtr resumeLoopThread;

        bool pongMark;
        bool helloMark;

        bool reconnectMark;
        bool timeoutMark;
        bool resumeMark;

        int resumeIntervalCnt;
        int resumeCnt;

        int s;
        int sn;
        int maxSn;

        string gatewayAPI;
        string websocketUrl;
        string authorization;
        int compress{};

        queue<unique_ptr<SignalBase>> signalQueue;

        [[nodiscard]] auto getGatewayUrl() -> int;

    public:
        BotInstance();

        ~BotInstance();

        void run();

    };

} // ZeroBot

#endif //ZEROBOT_BOTINSTANCE_HH
