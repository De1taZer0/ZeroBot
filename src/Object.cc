#include "Object.hh"

namespace ZeroBot::Object
{
    User::User(json &&rawMsg)
    {
        try
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
//            mobile_verified = rawMsg.at("mobile_verified").get<bool>();
            roles.clear();
            for(auto& i : rawMsg.at("roles").get<json>())
            {
                roles.push_back(i.get<int>());
            }
        }
        catch(const std::exception& e)
        {
            std::cout << "Error occurred when processing message: ";
            std::cout << "\n\t--In constructor of Object::User" << e.what() << std::endl;
            throw e;
        }
    }

    Role::Role(json &&rawMsg)
    {
        try
        {
            role_id = rawMsg.at("role_id").get<int>();
            name = rawMsg.at("name").get<string>();
            color = rawMsg.at("color").get<int>();
            position = rawMsg.at("position").get<int>();
            hoist = rawMsg.at("hoist").get<int>();
            mentionable = rawMsg.at("mentionable").get<int>();
            permissions = rawMsg.at("permissions").get<int>();
        }
        catch(const std::exception& e)
        {
            std::cout << "Error occurred when processing message: ";
            std::cout << "\n\t--In constructor of Object::Role" << e.what() << std::endl;
            throw e;
        }
    }

    Attachments::Attachments(json &&rawMsg)
    {
        try
        {
            type = rawMsg.at("type").get<string>();
            url = rawMsg.at("url").get<string>();
            name = rawMsg.at("name").get<string>();
            size = rawMsg.at("size").get<int>();
        }
        catch(const std::exception& e)
        {
            std::cout << "Error occurred when processing message: ";
            std::cout << "\n\t--In constructor of Object::Attachments" << e.what() << std::endl;
            throw e;
        }
    }

    KMarkdown::KMarkdown(json &&rawMsg)
    {
        try
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
        catch(const std::exception& e)
        {
            std::cout << "Error occurred when processing message: ";
            std::cout << "\n\t--In constructor of Object::KMarkdown" << e.what() << std::endl;
            throw e;
        }
    }
}