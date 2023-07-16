#ifndef ZEROBOT_SIGNAL_HH
#define ZEROBOT_SIGNAL_HH

///
/// @file Signal.hh
/// @date 2023.7.16
///

#include <string>
#include <format>
#include <memory>
#include <hv/json.hpp>

namespace ZeroBot::Signal
{
    using string = std::string;

    template<typename T>
    using unique_ptr = std::unique_ptr<T>;

    using json = nlohmann::json;

    enum class Sign:int
    {
        EVENT = 0,
        HELLO = 1,
        PING = 2,
        PONG = 3,
        RESUME = 4,
        RECONNECT = 5,
        RESUME_ACK = 6
    };

    class SignalBase
    {
    protected:
        unique_ptr<json> rawMessage;



    public:

        [[nodiscard]] static auto construct(unique_ptr<json> rawMsg) -> unique_ptr<SignalBase>;

        [[nodiscard]] virtual auto getType() -> Sign = 0;

        virtual ~SignalBase() = default;

    };

    class EventSignal final : public SignalBase
    {
    protected:


    public:
        EventSignal() = delete;

        explicit EventSignal(unique_ptr<json> rawMsg);

        auto getType() -> Sign final;
    };

    class HelloSignal final : public SignalBase
    {
    public:
        HelloSignal() = delete;

        explicit HelloSignal(unique_ptr<json> rawMsg);

        auto getType() -> Sign final;
    };

    class PingSignal final : public SignalBase
    {
    public:
        PingSignal() = delete;

        explicit PingSignal(unique_ptr<json> rawMsg);

        static auto rawString(const int& sn) -> string;

        auto getType() -> Sign final;
    };

    class PongSignal final : public SignalBase
    {
    public:
        PongSignal() = delete;

        explicit PongSignal(unique_ptr<json> rawMsg);

        auto getType() -> Sign final;
    };

    class ResumeSignal final : public SignalBase
    {
    public:
        ResumeSignal() = delete;

        explicit ResumeSignal(unique_ptr<json> rawMsg);

        auto getType() -> Sign final;
    };

    class ReconnectSignal final : public SignalBase
    {
    public:
        ReconnectSignal() = delete;

        explicit ReconnectSignal(unique_ptr<json> rawMsg);

        static auto rawString(const int& sn) -> string;

        auto getType() -> Sign final;
    };

    class ResumeAckSignal final : public SignalBase
    {
    public:
        ResumeAckSignal() = delete;

        explicit ResumeAckSignal(unique_ptr<json> rawMsg);

        auto getType() -> Sign final;
    };
}

#endif //ZEROBOT_SIGNAL_HH
