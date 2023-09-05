#ifndef ZEROBOT_MUSIC_HH
#define ZEROBOT_MUSIC_HH

///
/// @file Music.hh
/// @date 2023/7/24
///

#include <thread>
#include <fstream>
#include <shared_mutex>
#include <hv/requests.h>
#include <hv/HttpClient.h>
#include "Plugin.hh"

namespace ZeroBot::Plugin
{
    using json = nlohmann::json;
    using string = std::string;
    using Transmitter = Transmit::Transmitter;

    using Msg_Type = MessageExtra::Msg_Type;

    class PluginMusic final : public PluginBase
    {
    private:
        inline static std::unordered_map<string, string> musicSelectCard;

        inline static std::shared_mutex mapMutex;

    public:
        ~PluginMusic() final = default;

        auto update(const EventBase& msg) -> bool final;
    };
}

#endif //ZEROBOT_MUSIC_HH
