#include "plugin/Echo.hh"

namespace ZeroBot::Plugin
{
    auto PluginEcho::update(const EventBase& msg) -> bool
    {
        if(msg.content.find("/echo") == 0)
        {
            Transmitter::sendGroupMsg(msg.target_id, msg.content.substr(6,msg.content.length() - 6));
        }
        return true;
    }
}