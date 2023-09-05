#include <iostream>
#include "BotInstance.hh"
#include "Transmitter.hh"
#include "Plugin.hh"
#include "plugin/Echo.hh"
#include "plugin/Jrrp.hh"
#include "plugin/Music.hh"
#include "plugin/ImageSearch.hh"

const std::string self_id = "2946684318";

int main()
{
    using namespace ZeroBot;

    logger_set_level(hlog, LOG_LEVEL_DEBUG);

    SetConsoleOutputCP(65001);

    ZeroBot::Bot::BotInstance bot;

    ZeroBot::Transmit::Transmitter::init();

    std::vector<std::unique_ptr<Plugin::PluginBase>> plugins;

    plugins.emplace_back(std::make_unique<Plugin::PluginEcho>());
    plugins.emplace_back(std::make_unique<Plugin::PluginJrrp>());
    plugins.emplace_back(std::make_unique<Plugin::PluginMusic>());
    plugins.emplace_back(std::make_unique<Plugin::PluginImageSearch>());

    bot.onEvent<Event::EventGroupMsg>([&plugins](const auto& msg)
    {
        if(msg.author_id != self_id)
        {
            for(auto& i : plugins)
            {
                i->update(msg);
            }
        }
    });

    bot.onEvent<Event::EventPersonMsg>([&plugins](const auto& msg)
    {
        if(msg.author_id != self_id)
        {
            for(auto& i : plugins)
            {
                i->update(msg);
            }
        }
    });

    bot.onEvent<Event::EventBroadcastMsg>([&plugins](const auto& msg)
    {
        if(msg.author_id != self_id)
        {
            for(auto& i : plugins)
            {
                i->update(msg);
            }
        }
    });

    bot.run();

    return 0;
}