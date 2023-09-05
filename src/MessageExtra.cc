#include "MessageExtra.hh"

namespace ZeroBot::MessageExtra
{
    std::ostream& operator << (std::ostream& o,Msg_Type mType)
    {
        switch(mType)
        {
            case Msg_Type::TEXT:
                o << "Msg_Type::TEXT";
                break;
            case Msg_Type::IMAGE:
                o << "Msg_Type::IMAGE";
                break;
            case Msg_Type::VIDEO:
                o << "Msg_Type::VIDEO";
                break;
            case Msg_Type::FILE:
                o << "Msg_Type::FILE";
                break;
            case Msg_Type::SOUND:
                o << "Msg_Type::SOUND";
                break;
            case Msg_Type::KMARKDOWN:
                o << "Msg_Type::KMARKDOWN";
                break;
            case Msg_Type::CARD:
                o << "Msg_Type::CARD";
                break;
            case Msg_Type::SYSTEM:
                o << "Msg_Type::SYSTEM";
                break;
        }
        return o;
    }

    TextMsg::TextMsg(json&& rawMsg)
    {
        try
        {
            rawMessage = std::make_unique<json>(rawMsg);

            type = static_cast<Msg_Type>(rawMessage->at("type").get<int>());
            code = rawMessage->at("code").get<string>();
            guild_id = rawMessage->at("guild_id").get<string>();
            channel_name = rawMessage->at("channel_name").get<string>();
            mention.clear();
            for(auto& i : rawMessage->at("mention").get<json>())
            {
                mention.push_back(std::move(i.get<string>()));
            }
            mention_all = rawMessage->at("mention_all").get<bool>();
            mention_roles.clear();
            for(auto& i : rawMessage->at("mention_roles").get<json>())
            {
                mention_roles.emplace_back(std::move(i.get<json>()));
            }
            mention_here = rawMessage->at("mention_here").get<bool>();
            author = User(std::move(rawMessage->at("author").get<json>()));
        }
        catch(const std::exception& e)
        {
            std::cout << "\t--In constructor of TextMsg: " << e.what() << std::endl;
            throw e;
        }
    }

    ImageMsg::ImageMsg(json&& rawMsg)
    {
        try
        {
            rawMessage = std::make_unique<json>(rawMsg);

            type = static_cast<Msg_Type>(rawMessage->at("type").get<int>());
//            code = rawMessage->at("code").get<string>();
            guild_id = rawMessage->at("guild_id").get<string>();
            attachments.clear();
            attachments.emplace_back(std::move(rawMessage->at("attachments").get<json>()));
            author = User(std::move(rawMessage->at("author").get<json>()));
        }
        catch(const std::exception& e)
        {
            std::cout << "\t--In constructor of ImageMsg: " << e.what() << std::endl;
            throw e;
        }
    }

    VideoMsg::VideoMsg(json &&rawMsg)
    {
        try
        {
            rawMessage = std::make_unique<json>(rawMsg);

            type = static_cast<Msg_Type>(rawMessage->at("type").get<int>());
            code = rawMessage->at("code").get<string>();
            guild_id = rawMessage->at("guild_id").get<string>();
            attachments.clear();
            for(auto& i : rawMessage->at("attachments").get<json>())
            {
                attachments.emplace_back(std::move(i.get<json>()));
            }
            author = User(std::move(rawMessage->at("author").get<json>()));
        }
        catch(const std::exception& e)
        {
            std::cout << "\t--In constructor of VideoMsg: " << e.what() << std::endl;
            throw e;
        }
    }

    FileMsg::FileMsg(json &&rawMsg)
    {
        try
        {
            rawMessage = std::make_unique<json>(rawMsg);

            type = static_cast<Msg_Type>(rawMessage->at("type").get<int>());
            code = rawMessage->at("code").get<string>();
            guild_id = rawMessage->at("guild_id").get<string>();
            attachments.clear();
            for(auto& i : rawMessage->at("attachments").get<json>())
            {
                attachments.emplace_back(std::move(i.get<json>()));
            }
            author = User(std::move(rawMessage->at("author").get<json>()));
        }
        catch(const std::exception& e)
        {
            std::cout << "\t--In constructor of VideoMsg: " << e.what() << std::endl;
            throw e;
        }
    }

    SoundMsg::SoundMsg(json &&rawMsg)
    {
        try
        {
            rawMessage = std::make_unique<json>(rawMsg);

            type = static_cast<Msg_Type>(rawMessage->at("type").get<int>());
            code = rawMessage->at("code").get<string>();
            guild_id = rawMessage->at("guild_id").get<string>();
            attachments.clear();
            for(auto& i : rawMessage->at("attachments").get<json>())
            {
                attachments.emplace_back(std::move(i.get<json>()));
            }
            author = User(std::move(rawMessage->at("author").get<json>()));
        }
        catch(const std::exception& e)
        {
            std::cout << "\t--In constructor of SoundMsg: " << e.what() << std::endl;
            throw e;
        }
    }

    KMarkDownMsg::KMarkDownMsg(json &&rawMsg)
    {
        try
        {
            rawMessage = std::make_unique<json>(rawMsg);

            type = static_cast<Msg_Type>(rawMessage->at("type").get<int>());
            code = rawMessage->at("code").get<string>();
            guild_id = rawMessage->at("guild_id").get<string>();
            channel_name = rawMessage->at("channel_name").get<string>();
            mention.clear();
            for(auto& i : rawMessage->at("mention").get<json>())
            {
                mention.push_back(std::move(i.get<string>()));
            }
            mention_all = rawMessage->at("mention_all").get<bool>();
            mention_roles.clear();
            for(auto& i : rawMessage->at("mention_roles").get<json>())
            {
                mention_roles.emplace_back(std::move(i.get<json>()));
            }
            mention_here = rawMessage->at("mention_here").get<bool>();
            author = User(std::move(rawMessage->at("author").get<json>()));
            kmarkdown = KMarkdown(std::move(rawMessage->at("kmarkdown").get<json>()));
        }
        catch(const std::exception& e)
        {
            std::cout << "\t--In constructor of KMarkDownMsg: " << e.what() << std::endl;
            throw e;
        }
    }

    CardMsg::CardMsg(json &&rawMsg)
    {
        try
        {
            rawMessage = std::make_unique<json>(rawMsg);

            type = static_cast<Msg_Type>(rawMessage->at("type").get<int>());
            code = rawMessage->at("code").get<string>();
            guild_id = rawMessage->at("guild_id").get<string>();
            channel_name = rawMessage->at("channel_name").get<string>();
            mention.clear();
            for(auto& i : rawMessage->at("mention").get<json>())
            {
                mention.push_back(std::move(i.get<string>()));
            }
            mention_all = rawMessage->at("mention_all").get<bool>();
            mention_roles.clear();
            for(auto& i : rawMessage->at("mention_roles").get<json>())
            {
                mention_roles.emplace_back(std::move(i.get<json>()));
            }
            mention_here = rawMessage->at("mention_here").get<bool>();
            author = User(std::move(rawMessage->at("author").get<json>()));
        }
        catch(const std::exception& e)
        {
            std::cout << "\t--In constructor of CardMsg: " << e.what() << std::endl;
            throw e;
        }
    }

    ItemMsg::ItemMsg(json &&rawMsg)
    {
        try
        {
            rawMessage = std::make_unique<json>(rawMsg);

            type = static_cast<Msg_Type>(rawMessage->at("type").get<int>());
        }
        catch(const std::exception& e)
        {
            std::cout << "\t--In constructor of ItemMsg: " << e.what() << std::endl;
            throw e;
        }
    }

    SystemMsg::SystemMsg(json &&rawMsg)
    {
        try
        {
            rawMessage = std::make_unique<json>(rawMsg);

            type = rawMessage->at("type").get<string>();

            if(type == "message_btn_click")
            {
                body.msg_id = rawMessage->at("body").at("msg_id").get<string>();
                body.user_id = rawMessage->at("body").at("user_id").get<string>();
                body.value = rawMessage->at("body").at("value").get<string>();
                body.target_id = rawMessage->at("body").at("target_id").get<string>();
            }

        }
        catch(const std::exception& e)
        {
            std::cout << "\t--In constructor of SystemMsg: " << e.what() << std::endl;
            throw e;
        }
    }
}