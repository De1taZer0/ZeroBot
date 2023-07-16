#include "Signal.hh"

namespace ZeroBot::Signal
{
    auto SignalBase::construct(json&& rawMsg) -> unique_ptr<SignalBase>
    {
        try
        {
            switch(static_cast<Sign>(rawMsg.at("s").get<int>()))
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
        catch(const std::exception& e)
        {
            std::cout << "\t\tIn SignalBase::construct" << std::endl;
        }
    }

    auto SignalBase::getRawMsg() const noexcept -> json
    {
        return rawMessage->get<json>();
    }


    EventSignal::EventSignal(json&& rawMsg)
    {
        rawMessage = std::make_unique<json>(rawMsg);
    }

    auto EventSignal::getType() const noexcept -> Sign
    {
        return Sign::PING;
    }


    HelloSignal::HelloSignal(json&& rawMsg)
    {
        try
        {
            rawMessage = std::make_unique<json>(rawMsg);
        }
        catch(const std::exception& e)
        {
            std::cout << "Error occurred when processing message: " << rawMsg << std::endl;
            std::cout << "\t"  << e.what() << std::endl;
            std::cout << "\t\tIn HelloSignal::HelloSignal" << std::endl;
            throw e;
        }
    }

    auto HelloSignal::getType() const noexcept -> Sign
    {
        return Sign::HELLO;
    }


    PingSignal::PingSignal(json&& rawMsg)
    {
        rawMessage = std::make_unique<json>(rawMsg);
    }

    auto PingSignal::rawString(const int& sn) -> string
    {
        return json{ { "s", static_cast<int>(Sign::PING) },
                     { "sn", sn } }.dump();
    }

    auto PingSignal::getType() const noexcept -> Sign
    {
        return Sign::PING;
    }


    PongSignal::PongSignal(json&& rawMsg)
    {
        rawMessage = std::make_unique<json>(rawMsg);
    }

    auto PongSignal::getType() const noexcept -> Sign
    {
        return Sign::PONG;
    }


    ResumeSignal::ResumeSignal(json&& rawMsg)
    {
        rawMessage = std::make_unique<json>(rawMsg);
    }

    auto ResumeSignal::getType() const noexcept -> Sign
    {
        return Sign::RESUME;
    }


    ReconnectSignal::ReconnectSignal(json&& rawMsg)
    {
        rawMessage = std::make_unique<json>(rawMsg);
    }

    auto ReconnectSignal::rawString(const int &sn) -> string
    {
        return json{ { "s", static_cast<int>(Sign::RECONNECT) },
                     { "sn", sn } }.dump();
    }

    auto ReconnectSignal::getType() const noexcept -> Sign
    {
        return Sign::RECONNECT;
    }


    ResumeAckSignal::ResumeAckSignal(json&& rawMsg)
    {
        rawMessage = std::make_unique<json>(rawMsg);
    }

    auto ResumeAckSignal::getType() const noexcept -> Sign
    {
        return Sign::RESUME_ACK;
    }

}