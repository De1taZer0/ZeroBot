#ifndef ZEROBOT_IMAGESEARCH_HH
#define ZEROBOT_IMAGESEARCH_HH

///
/// @file ImageSearch.hh
/// @date 2023/7/22
///

#include <string>
#include <codecvt>
#include <locale>
#include <unordered_map>
#include <hv/json.hpp>
#include <hv/HttpClient.h>
#include "Plugin.hh"

namespace ZeroBot::Plugin
{
    using EventBase = Event::EventBase;
    using Transmitter = Transmit::Transmitter;
    using string = std::string;
    using json = nlohmann::json;

    using Msg_Type = MessageExtra::Msg_Type;

    class PluginImageSearch final : public PluginBase
    {
    private:
        inline static std::unordered_map<string, bool> imageSearchSign;

    public:
        ~PluginImageSearch() final = default;

        auto update(const EventBase& msg) -> bool final;
    };

}

#endif //ZEROBOT_IMAGESEARCH_HH
