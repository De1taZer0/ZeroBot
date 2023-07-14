#ifndef ZEROBOT_BOTINSTANCE_HH
#define ZEROBOT_BOTINSTANCE_HH

///
/// @file BotInstance.hh
/// @date 2023.7.15
///

#include <functional>
#include <hv/json.hpp>
#include <hv/requests.h>
#include <hv/WebSocketClient.h>
#include "Setting.hh"
#include "Event.hh"

namespace ZeroBot::Bot
{

    class BotInstance
    {
    private:
        hv::WebSocketClient wsCli;



    public:
        BotInstance() = default;

        ~BotInstance();

    };

} // ZeroBot

#endif //ZEROBOT_BOTINSTANCE_HH
