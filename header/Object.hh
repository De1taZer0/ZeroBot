#ifndef ZEROBOT_OBJECT_HH
#define ZEROBOT_OBJECT_HH

///
/// @file Object.hh
/// @date 2023.7.15
///

#include <string>
#include <vector>
#include <hv/json.hpp>

namespace ZeroBot::Object
{
    using string = std::string;

    template<typename T>
    using array = std::vector<T>;

    using json = nlohmann::json;

    struct User;
    struct Guild;
    struct Role;
    struct Channel;
    struct Quote;
    struct Attachments;

    ///
    /// @struct User
    ///
    struct User
    {
        string id; ///< 用户的 id
        string username; ///< 用户的名称
        string nickname; ///< 用户在当前服务器的昵称
        string identify_num; ///< 用户名的认证数字，用户名正常为：user_name#identify_num
        bool online; ///< 当前是否在线
        bool bot; ///< 是否为机器人
        int status; ///< 用户的状态, 0 和 1 代表正常，10 代表被封禁
        string avatar; ///< 用户的头像的 url 地址
        string vip_avatar; ///< vip 用户的头像的 url 地址，可能为 gif 动图
        bool mobile_verified; ///< 是否手机号已验证
        array<int> roles; ///< 用户在当前服务器中的角色 id 组成的列表
    };

    ///
    /// @struct Guild
    ///
    struct Guild
    {
        string id; ///< 服务器 id
        string name; ///< 服务器名称
        string topic; ///< 服务器主题
        string user_id; ///< 服务器主的 id
        string icon; ///< 服务器 icon 的地址
        int notify_type; ///< 通知类型, 0代表默认使用服务器通知设置，1代表接收所有通知, 2代表仅@被提及，3代表不接收通知
        string region; ///< 服务器默认使用语音区域
        bool enable_open; ///< 是否为公开服务器
        string open_id; ///< 公开服务器 id
        string default_channel_id; ///< 默认频道 id
        string welcome_channel_id; ///< 欢迎频道 id
        array<Role> roles; ///< 角色列表
        array<Channel> channels; ///< 频道列表
    };

    ///
    /// @struct Role
    ///
    struct Role
    {
        int role_id; ///< 角色 id
        string name; ///< 角色名称
        int color; ///< 颜色色值
        int position; ///< 顺序位置
        int hoist; ///< 是否为角色设定(与普通成员分开显示)
        int mentionable; ///< 是否允许任何人\@提及此角色
        int permissions; ///< 权限码
    };

    ///
    /// @struct RolePermission
    ///
    struct RolePermission : Role
    {
        int allow{};
        int deny{};
    };

    ///
    /// @struct UserPermission
    ///
    struct UserPermission : User
    {
        int allow{};
        int deny{};
    };

    ///
    /// @struct Channel
    ///
    struct Channel
    {
        string id; ///< 频道 id
        string name; ///< 频道名称
        string user_id; ///< 创建者 id
        string guild_id; ///< 服务器 id
        string topic; ///< 频道简介
        bool is_category; ///< 是否为分组，事件中为 int 格式
        string parent_id; ///< 上级分组的 id (若没有则为 0 或空字符串)
        int level; ///< 排序 level
        int slow_mode; ///< 慢速模式下限制发言的最短时间间隔, 单位为秒(s)
        int type; ///< 频道类型: 1 文字频道, 2 语音频道
        array<RolePermission> permission_overwrites; ///< 针对角色在该频道的权限覆写规则组成的列表
        array<UserPermission> permission_users; ///< 针对用户在该频道的权限覆写规则组成的列表
        int permission_sync; ///< 权限设置是否与分组同步, 1 or 0
        bool has_password; ///< 是否有密码
    };

    ///
    /// @struct Quote
    ///
    struct Quote
    {
        string id; ///< 引用消息 id
        int type; ///< 引用消息类型
        string content; ///< 引用消息内容
        int create_at; ///< 引用消息创建时间（毫秒）
        json author; ///< 作者的用户信息
    };

    ///
    /// @struct Attachments
    ///
    struct Attachments
    {
        string type; ///< 多媒体类型
        string url; ///< 多媒体地址
        string name; ///< 多媒体名
        int size; ///< 大小 单位 Byte
    };

    ///
    /// @struct KMarkDown
    ///
    struct KMarkDown
    {
        string raw_content;
        array<User> mention_part;
        array<Role> mention_role_part;
    };
}

#endif //ZEROBOT_OBJECT_HH
