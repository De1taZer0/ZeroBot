#ifndef ZEROBOT_ECHO_HH
#define ZEROBOT_ECHO_HH

///
/// @file Echo.hh
/// @date 2023.7.20
///

#include "Plugin.hh"

namespace ZeroBot::Plugin
{
    using Transmit::Transmitter;

    class PluginEcho : public PluginBase
    {
    public:
        auto update(const EventBase& event) -> bool final;
    };
}

#endif //ZEROBOT_ECHO_HH
