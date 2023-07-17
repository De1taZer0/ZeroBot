#include "Signal.hh"

namespace ZeroBot::Signal
{
    std::ostream& operator << (std::ostream& o, Sign sign)
    {
        switch(sign)
        {
            case Sign::EVENT:
                o << "EVENT";
                break;
            case Sign::HELLO:
                o << "HELLO";
                break;
            case Sign::PING:
                o << "PING";
                break;
            case Sign::PONG:
                o << "PONG";
                break;
            case Sign::RESUME:
                o << "RESUME";
                break;
            case Sign::RECONNECT:
                o << "RECONNECT";
                break;
            case Sign::RESUME_ACK:
                o << "RESUME_ACK";
                break;
        }
        return o;
    }

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
            std::cout << "\t--In SignalBase::construct: " << e.what() << std::endl;
            throw e;
        }
    }

    auto SignalBase::getRawMsg() const noexcept -> json
    {
        return rawMessage->get<json>();
    }


    EventSignal::EventSignal(json&& rawMsg)
    {
        try
        {
            rawMessage = std::make_unique<json>(rawMsg);
        }
        catch(const std::exception& e)
        {
            std::cout << "Error occurred when processing message: ";
            std::cout << "\n\t--In constructor of EventSignal" << e.what() << std::endl;
            throw e;
        }
    }

    auto EventSignal::getType() const noexcept -> Sign
    {
        return Sign::EVENT;
    }


    HelloSignal::HelloSignal(json&& rawMsg)
    {
        try
        {
            rawMessage = std::make_unique<json>(rawMsg);
        }
        catch(const std::exception& e)
        {
            std::cout << "Error occurred when processing message: ";
            std::cout << "\n\t--In constructor of HelloSignal" << e.what() << std::endl;
            throw e;
        }
    }

    auto HelloSignal::getType() const noexcept -> Sign
    {
        return Sign::HELLO;
    }


    PingSignal::PingSignal(json&& rawMsg)
    {
        try
        {
            rawMessage = std::make_unique<json>(rawMsg);
        }
        catch(const std::exception& e)
        {
            std::cout << "Error occurred when processing message: ";
            std::cout << "\n\t--In constructor of PingSignal" << e.what() << std::endl;
            throw e;
        }
    }

    auto PingSignal::rawString(const int& sn) -> string
    {
        return nlohmann::to_string(json{ { "s", static_cast<int>(Sign::PING) },{ "sn", sn } });
    }

    auto PingSignal::getType() const noexcept -> Sign
    {
        return Sign::PING;
    }


    PongSignal::PongSignal(json&& rawMsg)
    {
        try
        {
            rawMessage = std::make_unique<json>(rawMsg);
        }
        catch(const std::exception& e)
        {
            std::cout << "Error occurred when processing message: ";
            std::cout << "\n\t--In constructor of PongSignal: " << e.what() << std::endl;
            throw e;
        }
    }

    auto PongSignal::getType() const noexcept -> Sign
    {
        return Sign::PONG;
    }


    ResumeSignal::ResumeSignal(json&& rawMsg)
    {
        try
        {
            rawMessage = std::make_unique<json>(rawMsg);
        }
        catch(const std::exception& e)
        {
            std::cout << "Error occurred when processing message: ";
            std::cout << "\n\t--In constructor of ResumeSignal: " << e.what() << std::endl;
            throw e;
        }
    }

    auto ResumeSignal::getType() const noexcept -> Sign
    {
        return Sign::RESUME;
    }


    ReconnectSignal::ReconnectSignal(json&& rawMsg)
    {
        try
        {
            rawMessage = std::make_unique<json>(rawMsg);
        }
        catch(const std::exception& e)
        {
            std::cout << "Error occurred when processing message: ";
            std::cout << "\n\t--In constructor of ReconnectSignal: " << e.what() << std::endl;
            throw e;
        }
    }

    auto ReconnectSignal::rawString(const int &sn) -> string
    {
        return nlohmann::to_string(json{ { "s", static_cast<int>(Sign::RECONNECT) },{ "sn", sn } });
    }

    auto ReconnectSignal::getType() const noexcept -> Sign
    {
        return Sign::RECONNECT;
    }


    ResumeAckSignal::ResumeAckSignal(json&& rawMsg)
    {
        try
        {
            rawMessage = std::make_unique<json>(rawMsg);
        }
        catch(const std::exception& e)
        {
            std::cout << "Error occurred when processing message: ";
            std::cout << "\n\t--In constructor of ResumeAckSignal: " << e.what() << std::endl;
            throw e;
        }
    }

    auto ResumeAckSignal::getType() const noexcept -> Sign
    {
        return Sign::RESUME_ACK;
    }

}