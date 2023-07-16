#include "Signal.hh"

namespace ZeroBot::Signal
{
    auto SignalBase::construct(unique_ptr<ZeroBot::Signal::json> rawMsg) -> unique_ptr<SignalBase>
    {
        switch(Sign(rawMsg->at("s").get<int>()))
        {
            case Sign::EVENT:
                return std::make_unique<EventSignal>(std::move(rawMsg));
            case Sign::HELLO:
                return std::make_unique<HelloSignal>(std::move(rawMsg));
            case Sign::PING:
                return std::make_unique<PingSignal>(std::move(rawMsg));
            case Sign::PONG:
                return std::make_unique<PongSignal>(std::move(rawMsg));
            case Sign::RESUME:
                return std::make_unique<ResumeSignal>(std::move(rawMsg));
            case Sign::RECONNECT:
                return std::make_unique<ReconnectSignal>(std::move(rawMsg));
            case Sign::RESUME_ACK:
                return std::make_unique<ResumeAckSignal>(std::move(rawMsg));
        }
    }


    EventSignal::EventSignal(unique_ptr<json> rawMsg)
    {
        rawMessage = std::move(rawMsg);
    }

    auto EventSignal::getType() -> Sign
    {
        return Sign::PING;
    }


    HelloSignal::HelloSignal(unique_ptr<json> rawMsg)
    {
        rawMessage = std::move(rawMsg);
    }

    auto HelloSignal::getType() -> Sign
    {
        return Sign::HELLO;
    }


    PingSignal::PingSignal(unique_ptr<json> rawMsg)
    {
        rawMessage = std::move(rawMsg);
    }

    auto PingSignal::rawString(const int& sn) -> string
    {
        return json{ { "s", static_cast<int>(Sign::PING) },
                     { "sn", sn } }.dump();
    }

    auto PingSignal::getType() -> Sign
    {
        return Sign::PING;
    }


    PongSignal::PongSignal(unique_ptr<json> rawMsg)
    {
        rawMessage = std::move(rawMsg);
    }

    auto PongSignal::getType() -> Sign
    {
        return Sign::PONG;
    }


    ResumeSignal::ResumeSignal(unique_ptr<json> rawMsg)
    {
        rawMessage = std::move(rawMsg);
    }

    auto ResumeSignal::getType() -> Sign
    {
        return Sign::RESUME;
    }


    ReconnectSignal::ReconnectSignal(unique_ptr<json> rawMsg)
    {
        rawMessage = std::move(rawMsg);
    }

    auto ReconnectSignal::rawString(const int &sn) -> string
    {
        return json{ { "s", static_cast<int>(Sign::RECONNECT) },
                     { "sn", sn } }.dump();
    }

    auto ReconnectSignal::getType() -> Sign
    {
        return Sign::RECONNECT;
    }


    ResumeAckSignal::ResumeAckSignal(unique_ptr<json> rawMsg)
    {
        rawMessage = std::move(rawMsg);
    }

    auto ResumeAckSignal::getType() -> Sign
    {
        return Sign::RESUME_ACK;
    }

}