#include "Event.hh"

namespace ZeroBot::Event
{
    int EventBase::maxSN = 0;

    std::ostream& operator << (std::ostream& o,Channel_Type cType)
    {
        switch(cType)
        {
            case Channel_Type::GROUP:
                o << "Channel_Type::GROUP";
                break;
            case Channel_Type::PERSON:
                o << "Channel_Type::PERSON";
                break;
            case Channel_Type::BROADCAST:
                o << "Channel_Type::BROADCAST";
                break;
        }
        return o;
    }

    auto EventBase::construct(json&& rawMsg) -> std::unique_ptr<EventBase>
    {
        switch(cTypeMap.at(rawMsg.at("channel_type").get<std::string>()))
        {
            case Channel_Type::BROADCAST:
                return std::make_unique<EventBroadcastMsg>(std::move(rawMsg));
            case Channel_Type::PERSON:
                return std::make_unique<EventPersonMsg>(std::move(rawMsg));
            case Channel_Type::GROUP:
                return std::make_unique<EventGroupMsg>(std::move(rawMsg));
        }
    }

    bool operator < (const EventBase& eventA, const EventBase& eventB)
    {
        return (eventA.sn > eventB.sn);
    }

    auto EventBase::getMaxSN() noexcept -> int
    {
        return maxSN;
    }

    auto EventBase::resetMaxSN() noexcept -> void
    {
        maxSN = 0;
    }

    EventGroupMsg::EventGroupMsg(json&& rawMsg)
    {
        sn = rawMsg.at("sn").get<int>();

        maxSN = (sn > maxSN) ? sn : maxSN;
        if(maxSN == 65535)
        {
            resetMaxSN();
        }

        rawMessage = std::make_unique<json>(rawMsg.at("d").get<json>());

        type = static_cast<Msg_Type>(rawMessage->at("type").get<int>());

        switch(type)
        {
            case Msg_Type::TEXT:
                extra = std::make_unique<MessageExtra::TextMsg>(std::move(rawMessage->at("extra")));
                break;
            case Msg_Type::IMAGE:
                extra = std::make_unique<MessageExtra::ImageMsg>(std::move(rawMessage->at("extra")));
                break;
            case Msg_Type::VIDEO:
                extra = std::make_unique<MessageExtra::VideoMsg>(std::move(rawMessage->at("extra")));
                break;
            case Msg_Type::FILE:
                extra = std::make_unique<MessageExtra::FileMsg>(std::move(rawMessage->at("extra")));
                break;
            case Msg_Type::SOUND:
                extra = std::make_unique<MessageExtra::SoundMsg>(std::move(rawMessage->at("extra")));
                break;
            case Msg_Type::KMARKDOWN:
                extra = std::make_unique<MessageExtra::KMarkDownMsg>(std::move(rawMessage->at("extra")));
                break;
            case Msg_Type::CARD:
                extra = std::make_unique<MessageExtra::CardMsg>(std::move(rawMessage->at("extra")));
                break;
            case Msg_Type::SYSTEM:
                extra = std::make_unique<MessageExtra::SystemMsg>(std::move(rawMessage->at("extra")));
                break;
        }
    }

    EventGroupMsg::~EventGroupMsg() = default;

    auto EventGroupMsg::getType() const noexcept -> Event_Type
    {
        return Event_Type{ Channel_Type::GROUP, type };
    }

    EventPersonMsg::EventPersonMsg(json&& rawMsg)
    {
        sn = rawMsg.at("sn").get<int>();

        maxSN = (sn > maxSN) ? sn : maxSN;
        if(maxSN == 65535)
        {
            resetMaxSN();
        }

        rawMessage = std::make_unique<json>(rawMsg);

        type = Msg_Type(rawMessage->at("type").get<int>());

        switch(type)
        {
            case Msg_Type::TEXT:
            case Msg_Type::IMAGE:
            case Msg_Type::VIDEO:
            case Msg_Type::FILE:
            case Msg_Type::SOUND:
            case Msg_Type::KMARKDOWN:
            case Msg_Type::CARD:
            case Msg_Type::SYSTEM:
                break;
        }
    }

    EventPersonMsg::~EventPersonMsg() = default;

    auto EventPersonMsg::getType() const noexcept -> Event_Type
    {
        return Event_Type{ Channel_Type::PERSON, type };
    }

    EventBroadcastMsg::EventBroadcastMsg(json&& rawMsg)
    {
        sn = rawMsg.at("sn").get<int>();

        maxSN = (sn > maxSN) ? sn : maxSN;
        if(maxSN == 65535)
        {
            resetMaxSN();
        }

        rawMessage = std::make_unique<json>(rawMsg);

        type = Msg_Type(rawMessage->at("type").get<int>());

        switch(type)
        {
            case Msg_Type::TEXT:
            case Msg_Type::IMAGE:
            case Msg_Type::VIDEO:
            case Msg_Type::FILE:
            case Msg_Type::SOUND:
            case Msg_Type::KMARKDOWN:
            case Msg_Type::CARD:
            case Msg_Type::SYSTEM:
                break;
        }
    }

    EventBroadcastMsg::~EventBroadcastMsg() = default;

    auto EventBroadcastMsg::getType() const noexcept -> Event_Type
    {
        return Event_Type{ Channel_Type::BROADCAST, type };
    }

}