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
    template<typename T>
    using optional = std::optional<T>;

    using string = std::string;
    using ocrint = optional<std::reference_wrapper<const int>>;
    using crstring = const string&;
    using ocrstring = optional<std::reference_wrapper<const string>>;

    using json = nlohmann::json;

    class Transmitter
    {
    private:
        struct
        {
            int limit; ///< "X-Rate-Limit-Limit"
            int remaining; ///< "X-Rate-Limit-Remaining"
            int reset; ///< "X-Rate-Limit-Reset"
            int bucket; ///< "X-Rate-Limit-Bucket"
            int global; ///< "X-Rate-Limit-Global"
        }rateLimit;

        static string authorization;

        auto sendHttpRequest(HttpRequestPtr req) -> json;

    public:
        Transmitter() = default;

        static auto init() -> void;

        auto sendGroupMsg(crstring target_id, crstring content, ocrint type = std::nullopt, ocrstring quote = std::nullopt, ocrstring temp_target_id = std::nullopt) -> optional<std::pair<string, int>>;
    };
}

#endif //ZEROBOT_TRANSMITTER_HH
