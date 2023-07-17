#include <iostream>
#include "Event.hh"
#include "BotInstance.hh"

int main()
{
    using namespace ZeroBot;

    logger_set_level(hlog, LOG_LEVEL_DEBUG);

    SetConsoleOutputCP(65001);

    ZeroBot::Bot::BotInstance bot;

    bot.onEvent<Event::EventGroupMsg>([](const auto& msg)
    {
        std::cout << "\\\\\\\\GroupMsg:" << msg.content;
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