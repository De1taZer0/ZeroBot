#include "plugin/Jrrp.hh"

const std::string self_id = "2946684318";

namespace ZeroBot::Plugin
{
    auto PluginJrrp::update(const EventBase& msg) -> bool
    {
        if(msg.getType().mType != Msg_Type::SYSTEM && msg.content.find("/jrrp") == 0)
        {
            hash<string> hash;

            auto rp = static_cast<double>(hash(self_id + msg.author_id + std::to_string(msg.msg_timestamp / 86400000))) / SIZE_MAX * 100;

            string resMsg = "今日人品为:" + std::to_string(rp);

            Transmitter::sendGroupMsg(msg.target_id, resMsg, std::nullopt, std::optional(msg.msg_id));
        }
        return true;
    }
}