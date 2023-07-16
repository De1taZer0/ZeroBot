#ifndef ZEROBOT_MESSAGEEXTRA_HH
#define ZEROBOT_MESSAGEEXTRA_HH

///
/// @file MessageExtra.hh
/// @date 2023.7.15
///

#include <string>
#include <vector>
#include <hv/json.hpp>
#include "Object.hh"

namespace ZeroBot::MessageExtra
{
    using string = std::string;

    template<typename T>
    using array = std::vector<T>;

    template<typename T>
    using unique_ptr = std::unique_ptr<T>;

    using User = Object::User;
    using Role = Object::Role;
    using Channel = Object::Channel;
    using KMarkdown = Object::KMarkdown;
    using Attachments = Object::Attachments;

    using json = nlohmann::json;

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

    std::ostream& operator << (std::ostream& o,Msg_Type mType);

    class MessageExtraBase
    {
    public:
        unique_ptr<json> rawMessage;
    };

    class TextMsg : public MessageExtraBase
    {
    public:
        Msg_Type type;
        string code; ///<
        string guild_id; ///< 服务器 id
        string channel_name; ///< 频道名
        array<string> mention; ///< 提及到的用户 id 的列表
        bool mention_all; ///< 是否 mention 所有用户
        array<Role> mention_roles; ///< mention 用户角色的数组
        bool mention_here; ///< 是否 mention 在线用户
        User author; ///<

        TextMsg() = delete;

        explicit TextMsg(json&& rawMsg);
    };

    class ImageMsg : public MessageExtraBase
    {
    public:
        Msg_Type type;
        string code; ///<
        string guild_id; ///< 服务器 id
        array<Attachments> attachments; ///< 附件
        User author; ///< 用户信息

        ImageMsg() = delete;

        explicit ImageMsg(json&& rawMsg);
    };

    class VideoMsg : public MessageExtraBase
    {
    public:
        Msg_Type type;
        string code; ///<
        string guild_id; ///< 服务器 id
        array<Attachments> attachments; ///< 附件
        User author; ///< 用户信息

        VideoMsg() = delete;

        explicit VideoMsg(json&& rawMsg);
    };

    class FileMsg : public MessageExtraBase
    {
    public:
        Msg_Type type;
        string code; ///<
        string guild_id; ///< 服务器 id
        array<Attachments> attachments; ///< 附件
        User author; ///< 用户信息

        FileMsg() = delete;

        explicit FileMsg(json&& rawMsg);
    };

    class SoundMsg : public MessageExtraBase
    {
    public:
        Msg_Type type;
        string code; ///<
        string guild_id; ///< 服务器 id
        array<Attachments> attachments; ///< 附件
        User author; ///< 用户信息

        SoundMsg() = delete;

        explicit SoundMsg(json&& rawMsg);
    };

    class KMarkDownMsg : public MessageExtraBase
    {
    public:
        Msg_Type type;
        string code; ///<
        string guild_id; ///< 服务器 id
        string channel_name; ///< 频道名
        array<string> mention; ///< 提及到的用户 id 的列表
        bool mention_all; ///< 是否 mention 所有用户
        array<Role> mention_roles; ///< mention 用户角色的数组
        bool mention_here; ///< 是否 mention 在线用户
        array<Channel> nav_channels; ///<
        User author; ///< 用户信息
        KMarkdown kmarkdown; ///<

        KMarkDownMsg() = delete;

        explicit KMarkDownMsg(json&& rawMsg);
    };

    class CardMsg : public MessageExtraBase
    {
    public:
        Msg_Type type;
        string code; ///<
        string guild_id; ///< 服务器 id
        string channel_name; ///< 频道名
        array<string> mention; ///< 提及到的用户 id 的列表
        bool mention_all; ///< 是否 mention 所有用户
        array<Role> mention_roles; ///< mention 用户角色的数组
        bool mention_here; ///< 是否 mention 在线用户
        User author; ///< 用户信息
        array<Channel> nav_channels; ///<

        CardMsg() = delete;

        explicit CardMsg(json&& rawMsg);
    };

    class ItemMsg : public MessageExtraBase
    {
    public:
        Msg_Type type;

        ItemMsg() = delete;

        explicit ItemMsg(json&& rawMsg);
    };

    class SystemMsg : public MessageExtraBase
    {
    public:
        string type;

        SystemMsg() = delete;

        explicit SystemMsg(json&& rawMsg);
    };

}

#endif //ZEROBOT_MESSAGEEXTRA_HH
