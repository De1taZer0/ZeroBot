#ifndef ZEROBOT_EVENT_HH
#define ZEROBOT_EVENT_HH

///
/// @file Events.hh
/// @date 2023.7.15
///

#include <unordered_map>
#include <string>
#include <hv/json.hpp>

namespace ZeroBot::Event
{
    enum class Channel_Type:int
    {
        GROUP = 1,
        PERSON = 2,
        BROADCAST = 3
    };

    enum class Msg_Type:int
    {
        TEXT = 1,
        IMAGE = 2,
        VIDEO = 3,
        FILE = 4,
        SOUND = 8,
        KMARKDOWN = 9,
        CARD = 10,
        SYSTEM = 255
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
        nlohmann::json rawMsg;

        virtual ~EventBase() = default;

        [[nodiscard]] virtual Event_Type getType() const = 0;

        [[nodiscard]] static std::unique_ptr<EventBase> construct(const Event_Type& eventType);

        virtual void parse() = 0;

    };
}

#endif //ZEROBOT_EVENT_HH
