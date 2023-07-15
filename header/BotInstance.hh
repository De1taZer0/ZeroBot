#ifndef ZEROBOT_BOTINSTANCE_HH
#define ZEROBOT_BOTINSTANCE_HH

///
/// @file BotInstance.hh
/// @date 2023.7.15
///

#include <string>
#include <functional>
#include <hv/json.hpp>
#include <hv/requests.h>
#include <hv/WebSocketClient.h>
#include "Setting.hh"

namespace ZeroBot::Bot
{
    using string = std::string;

    using json = nlohmann::json;

    class BotInstance
    {
    private:
        hv::WebSocketClient wsCli;
        int s;
        int sn;
        int maxSn;

        string gateway;
        string authorization;
        int compress;

    public:
        BotInstance();

        ~BotInstance();

    };

} // ZeroBot

#endif //ZEROBOT_BOTINSTANCE_HH
