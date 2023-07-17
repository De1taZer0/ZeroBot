#ifndef ZEROBOT_SIGNAL_HH
#define ZEROBOT_SIGNAL_HH

///
/// @file Signal.hh
/// @date 2023.7.16
///

#include <string>
#include <format>
#include <memory>
#include <iostream>
#include <exception>
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

    std::ostream& operator << (std::ostream& o, Sign sign);

    class SignalBase
    {
    protected:
        unique_ptr<json> rawMessage;

    public:

        [[nodiscard]] static auto construct(json&& rawMsg) -> unique_ptr<SignalBase>;

        [[nodiscard]] virtual auto getType() const noexcept -> Sign = 0;

        [[nodiscard]] auto getRawMsg() const noexcept -> json;

        virtual ~SignalBase() = default;

    };

    class EventSignal final : public SignalBase
    {
    protected:


    public:
        EventSignal() = delete;

        explicit EventSignal(json&& rawMsg);

        [[nodiscard]] auto getType() const noexcept -> Sign final;
    };

    class HelloSignal final : public SignalBase
    {
    public:
        HelloSignal() = delete;

        explicit HelloSignal(json&& rawMsg);

        [[nodiscard]] auto getType() const noexcept -> Sign final;
    };

    class PingSignal final : public SignalBase
    {
    public:
        PingSignal() = delete;

        explicit PingSignal(json&& rawMsg);

        static auto rawString(const int& sn) -> string;

        [[nodiscard]] auto getType() const noexcept -> Sign final;
    };

    class PongSignal final : public SignalBase
    {
    public:
        PongSignal() = delete;

        explicit PongSignal(json&& rawMsg);

        [[nodiscard]] auto getType() const noexcept -> Sign final;
    };

    class ResumeSignal final : public SignalBase
    {
    public:
        ResumeSignal() = delete;

        explicit ResumeSignal(json&& rawMsg);

        [[nodiscard]] auto getType() const noexcept -> Sign final;
    };

    class ReconnectSignal final : public SignalBase
    {
    public:
        ReconnectSignal() = delete;

        explicit ReconnectSignal(json&& rawMsg);

        static auto rawString(const int& sn) -> string;

        [[nodiscard]] auto getType() const noexcept -> Sign final;
    };

    class ResumeAckSignal final : public SignalBase
    {
    public:
        ResumeAckSignal() = delete;

        explicit ResumeAckSignal(json&& rawMsg);

        [[nodiscard]] auto getType() const noexcept -> Sign final;
    };
}

#endif //ZEROBOT_SIGNAL_HH
