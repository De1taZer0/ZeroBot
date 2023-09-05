#ifndef ZEROBOT_JRRP_HH
#define ZEROBOT_JRRP_HH

///
/// @file Jrrp.hh
/// @date 2023/7/27
///

#include <string>
#include <xhash>
#include "Plugin.hh"

namespace ZeroBot::Plugin
{
    template<typename T>
    using hash = std::hash<T>;
    using string = std::string;
    using Msg_Type = Event::Msg_Type;
    using Transmitter = Transmit::Transmitter;

    class PluginJrrp final : public PluginBase
    {
    public:
        ~PluginJrrp() final = default;

        auto update(const EventBase& msg) -> bool final;
    };
}

#endif //ZEROBOT_JRRP_HH
