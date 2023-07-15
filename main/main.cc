#include <iostream>
#include <hv/requests.h>
#include <hv/WebSocketClient.h>
#include <hv/json.hpp>
#include "BotInstance.hh"

int main()
{
    using nlohmann::json;

    logger_set_level(hlog, LOG_LEVEL_DEBUG);

    SetConsoleOutputCP(65001);

    ZeroBot::Bot::BotInstance bot;



    return 0;
}