#include "Event.hh"

namespace ZeroBot::Event
{
    auto EventBase::construct(std::unique_ptr<nlohmann::json> rawMsg) -> std::unique_ptr<EventBase>
    {

        switch(cTypeMap.at(rawMsg->at("channel_type").get<std::string>()))
        {
            case Channel_Type::BROADCAST:
                return std::make_unique<GroupMsg>(std::move(rawMsg));
            case Channel_Type::PERSON:
                return std::make_unique<GroupMsg>(std::move(rawMsg));
            case Channel_Type::GROUP:
                return std::make_unique<GroupMsg>(std::move(rawMsg));
        }
    }

    GroupMsg::GroupMsg(std::unique_ptr<nlohmann::json> rawMsg)
    {
        rawMessage = std::move(rawMsg);

        msgType = Msg_Type(rawMsg->at("type").get<int>());

        switch(msgType)
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

    GroupMsg::~GroupMsg() = default;

    Event_Type GroupMsg::getType() const
    {
        return Event_Type{ Channel_Type::GROUP, msgType };
    }

}