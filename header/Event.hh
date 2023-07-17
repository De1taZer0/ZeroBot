#ifndef ZEROBOT_EVENT_HH
#define ZEROBOT_EVENT_HH

///
/// @file Events.hh
/// @date 2023.7.15
///

#include <string>
#include <iostream>
#include <exception>
#include <unordered_map>
#include <hv/json.hpp>
#include "Object.hh"
#include "MessageExtra.hh"

namespace ZeroBot::Event
{
    using string = std::string;

    template<typename T>
    using array = std::vector<T>;

    template<typename T>
    using unique_ptr = std::unique_ptr<T>;

    using json = nlohmann::json;

    using Msg_Type = MessageExtra::Msg_Type;

    enum class Channel_Type:int
    {
        GROUP = 1,
        PERSON = 2,
        BROADCAST = 3
    };

    std::ostream& operator << (std::ostream& o,Channel_Type cType);

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

    struct EventHash
    {
        auto operator()(const Event_Type &event) const -> size_t
        {
            return std::hash<Msg_Type>()(event.mType) ^ std::hash<Channel_Type>()(event.cType);
        }
    };

    class EventBase
    {
    protected:
        static int maxSN;

        unique_ptr<json> rawMessage;

        int sn{};

    public:
        Msg_Type type; ///<
        string target_id; ///< 发送目的, 频道消息类时, 代表的是频道 channel_id，如果 channel_type 为 GROUP 组播且 type 为 255 系统消息时，则代表服务器 guild_id
        string author_id; ///< 发送者 id, 1 代表系统
        string content; ///< 消息内容, 文件，图片，视频时，content 为 url
        string msg_id; ///< 消息的 id
        int msg_timestamp; ///< 消息发送时间的毫秒时间戳
        string nonce; ///< 随机串，与用户消息发送 api 中传的 nonce 保持一致
        unique_ptr<MessageExtra::MessageExtraBase> extra; ///< 不同的消息类型，结构不一致

        EventBase() = default;

        virtual ~EventBase() = default;

        [[nodiscard]] virtual Event_Type getType() const = 0;

        [[nodiscard]] static auto construct(json&& rawMsg) -> std::unique_ptr<EventBase>;

        [[nodiscard]] static auto getMaxSN() noexcept -> int;

        static auto resetMaxSN() noexcept -> void;

        friend bool operator < (const EventBase& eventA, const EventBase& eventB);
    };

    class EventGroupMsg final : public EventBase
    {
    protected:


    public:
        EventGroupMsg() = default;

        explicit EventGroupMsg(json&& rawMsg);

        ~EventGroupMsg() final;

        [[nodiscard]] auto getType() const noexcept -> Event_Type final;

    };

    class EventPersonMsg final : public EventBase
    {
    protected:


    public:
        EventPersonMsg() = default;

        explicit EventPersonMsg(json&& rawMsg);

        ~EventPersonMsg() final;

        [[nodiscard]] auto getType() const noexcept -> Event_Type final;

    };

    class EventBroadcastMsg final : public EventBase
    {
    protected:


    public:
        EventBroadcastMsg() = default;

        explicit EventBroadcastMsg(json&& rawMsg);

        ~EventBroadcastMsg() final;

        [[nodiscard]] auto getType() const noexcept -> Event_Type final;

    };

}

#endif //ZEROBOT_EVENT_HH
