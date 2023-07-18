#ifndef ZEROBOT_TRANSMITTER_HH
#define ZEROBOT_TRANSMITTER_HH

///
/// @file Transmitter.hh
/// @date 2023.7.20
///

#include <hv/requests.h>
#include <hv/json.hpp>
#include "Setting.hh"

namespace ZeroBot::Transmit
{
    using string = std::string;

    using json = nlohmann::json;

    class Transmitter
    {
    private:
        static string authorization;

    public:
        static auto init() -> void;

        static auto sendGroupMsg(const string& target_id, const string& content) -> std::pair<string, int>;

    };
}

#endif //ZEROBOT_TRANSMITTER_HH
