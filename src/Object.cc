#include "Object.hh"

namespace ZeroBot::Object
{
    User::User(json &&rawMsg)
    {
        id = rawMsg.at("id").get<string>();
        username = rawMsg.at("username").get<string>();
        nickname = rawMsg.at("nickname").get<string>();
        identify_num = rawMsg.at("identify_num").get<string>();
        online = rawMsg.at("online").get<bool>();
        bot = rawMsg.at("bot").get<bool>();
        status = rawMsg.at("status").get<int>();
        avatar = rawMsg.at("avatar").get<string>();
        vip_avatar = rawMsg.at("vip_avatar").get<string>();
        mobile_verified = rawMsg.at("mobile_verified").get<bool>();
        roles.clear();
        for(auto& i : rawMsg.at("roles").get<json>())
        {
            roles.push_back(i.get<int>());
        }
    }

    Role::Role(json &&rawMsg)
    {
        role_id = rawMsg.at("role_id").get<int>();
        name = rawMsg.at("name").get<string>();
        color = rawMsg.at("color").get<int>();
        position = rawMsg.at("position").get<int>();
        hoist = rawMsg.at("hoist").get<int>();
        mentionable = rawMsg.at("mentionable").get<int>();
        permissions = rawMsg.at("permissions").get<int>();
    }

    Attachments::Attachments(json &&rawMsg)
    {
        type = rawMsg.at("type").get<string>();
        url = rawMsg.at("url").get<string>();
        name = rawMsg.at("name").get<string>();
        size = rawMsg.at("size").get<int>();
    }

    KMarkdown::KMarkdown(json &&rawMsg)
    {
        raw_content = rawMsg.at("raw_content").get<string>();
        mention_part.clear();
        for(auto& i : rawMsg.at("mention_part").get<json>())
        {
            mention_part.emplace_back(std::move(i.get<json>()));
        }
        mention_role_part.clear();
        for(auto& i : rawMsg.at("mention_role_part").get<json>())
        {
            mention_role_part.emplace_back(std::move(i.get<json>()));
        }
    }
}