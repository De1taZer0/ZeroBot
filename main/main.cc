#include <iostream>
#include "BotInstance.hh"
#include "plugin/Echo.hh"

const std::string self_id = "2946684318";

int main()
{
    using namespace ZeroBot;

    logger_set_level(hlog, LOG_LEVEL_DEBUG);

    SetConsoleOutputCP(65001);

    ZeroBot::Bot::BotInstance bot;

    bot.onEvent<Event::EventGroupMsg>([&bot](const auto& msg)
    {
        if(msg.author_id != self_id)
        {
            if(msg.content.find("/echo") == 0)
            {
                bot.sendGroupMsg(msg.target_id, msg.content.substr(6,msg.content.length() - 6));
            }
        }
    });

    bot.onEvent<Event::EventPersonMsg>([](const auto& msg)
    {
        std::cout << "\\\\\\\\PersonMsg:" << msg.content;
    });

    bot.onEvent<Event::EventBroadcastMsg>([](const auto& msg)
    {
        std::cout << "\\\\\\\\BroadcastMsg:" << msg.content;
    });

    bot.run();

    return 0;
}