#ifndef ZEROBOT_MESSAGE_HH
#define ZEROBOT_MESSAGE_HH

///
/// @file Message.hh
/// @date 2023.7.15
///

#include <string>
#include <vector>
#include "Object.hh"

namespace ZeroBot::Message
{
    using string = std::string;

    template<typename T>
    using array = std::vector<T>;

    using User = Object::User;
    using Role = Object::Role;
    using Channel = Object::Channel;
    using KMarkDown = Object::KMarkDown;
    using Attachments = Object::Attachments;

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

    class MessageBase
    {
    public:
        Msg_Type type;
    };

    class TextMsg : public MessageBase
    {
    public:
        string code; ///<
        string guild_id; ///< 服务器 id
        string channel_name; ///< 频道名
        array<string> mention; ///< 提及到的用户 id 的列表
        bool mention_all; ///< 是否 mention 所有用户
        array<Role> mention_roles; ///< mention 用户角色的数组
        bool mention_here; ///< 是否 mention 在线用户
        User author; ///<
    };

    class ImageMsg : public MessageBase
    {
    public:
        string code; ///<
        string guild_id; ///< 服务器 id
        array<Attachments> attachments; ///< 附件
        User author; ///< 用户信息
    };

    class VideoMsg : public MessageBase
    {
    public:
        string code; ///<
        string guild_id; ///< 服务器 id
        array<Attachments> attachments; ///< 附件
        User author; ///< 用户信息
    };

    class FileMsg : public MessageBase
    {
    public:
        string code; ///<
        string guild_id; ///< 服务器 id
        array<Attachments> attachments; ///< 附件
        User author; ///< 用户信息
    };

    class KMarkDownMsg : public MessageBase
    {
    public:
        string code; ///<
        string guild_id; ///< 服务器 id
        string channel_name; ///< 频道名
        array<string> mention; ///< 提及到的用户 id 的列表
        bool mention_all; ///< 是否 mention 所有用户
        array<Role> mention_roles; ///< mention 用户角色的数组
        bool mention_here; ///< 是否 mention 在线用户
        array<Channel> nav_channels; ///<
        User author; ///< 用户信息
        KMarkDown kmarkdown; ///<
    };

    class CardMsg : public MessageBase
    {
        string code; ///<
        string guild_id; ///< 服务器 id
        string channel_name; ///< 频道名
        array<string> mention; ///< 提及到的用户 id 的列表
        bool mention_all; ///< 是否 mention 所有用户
        array<Role> mention_roles; ///< mention 用户角色的数组
        bool mention_here; ///< 是否 mention 在线用户
        array<Channel> nav_channels; ///<
        User author; ///< 用户信息
    };

    class ItemMsg : public MessageBase
    {

    };

}

#endif //ZEROBOT_MESSAGE_HH
