#ifndef ZEROBOT_EVENT_HH
#define ZEROBOT_EVENT_HH

///
/// @file Events.hh
/// @date 2023.7.15
///

#include <unordered_map>
#include <string>
#include <hv/json.hpp>
#include "Object.hh"
#include "Message.hh"

namespace ZeroBot::Event
{
    using string = std::string;

    template<typename T>
    using array = std::vector<T>;

    using Msg_Type = Message::Msg_Type;

    enum class Channel_Type:int
    {
        GROUP = 1,
        PERSON = 2,
        BROADCAST = 3
    };

    const std::unordered_map<string, Channel_Type> cTypeMap =
    {
        { "GROUP", Channel_Type::GROUP },
        { "PERSON", Channel_Type::PERSON },
        { "BROADCAST", Channel_Type::BROADCAST }
    };

    struct Event_Type
    {
        Channel_Type cType;
        Msg_Type mType;

        bool operator == (const Event_Type& other) const
        {
            return cType == other.cType && mType == other.mType;
        }
    };

    class EventBase
    {
    protected:
        std::unique_ptr<nlohmann::json> rawMsg;

    public:
        EventBase() = default;

        virtual ~EventBase() = default;

        [[nodiscard]] virtual Event_Type getType() const = 0;

        [[nodiscard]] static std::unique_ptr<EventBase> construct(std::unique_ptr<nlohmann::json> rawMsg);

    };

    class GroupMsg final : public EventBase
    {
    protected:
        Msg_Type msgType;

    public:
        GroupMsg() = delete;

        explicit GroupMsg(std::unique_ptr<nlohmann::json> rMsg);

        ~GroupMsg() final;

        [[nodiscard]] Event_Type getType() const final;

    };

}

#endif //ZEROBOT_EVENT_HH
