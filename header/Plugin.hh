#ifndef ZEROBOT_PLUGIN_HH
#define ZEROBOT_PLUGIN_HH

///
/// @file Plugin.hh
/// @date 2023.7.20
///

#include "Event.hh"
#include "Transmitter.hh"

namespace ZeroBot::Plugin
{
    using EventBase = Event::EventBase;

    class PluginBase
    {
    public:
        virtual auto update(const EventBase& event) -> bool = 0;

        PluginBase() = default;
        virtual ~PluginBase() = 0;
    };
}

#endif //ZEROBOT_PLUGIN_HH
