#ifndef ZEROBOT_IMAGESEARCH_HH
#define ZEROBOT_IMAGESEARCH_HH

///
/// @file ImageSearch.hh
/// @date 2023/7/22
///

#include <string>
#include <unordered_map>
#include <hv/json.hpp>
#include <hv/HttpClient.h>
#include "Plugin.hh"

const std::string apiKey = "0e89faf72ec6ea74f01bf07331d7a1b46c8469f3";

namespace ZeroBot::Plugin
{
    using EventBase = Event::EventBase;
    using Transmitter = Transmit::Transmitter;
    using string = std::string;

    class PluginImageSearch final : public PluginBase
    {
    private:
        static std::unordered_map<string, bool> imageSearchSign;

    public:
        ~PluginImageSearch() final = default;

        auto update(const EventBase& event) -> bool final;
    };

}

#endif //ZEROBOT_IMAGESEARCH_HH
