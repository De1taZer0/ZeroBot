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

    ImageMsg::ImageMsg(json&& rawMsg)
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

    VideoMsg::VideoMsg(json &&rawMsg)
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

    FileMsg::FileMsg(json &&rawMsg)
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

    SoundMsg::SoundMsg(json &&rawMsg)
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

    KMarkDownMsg::KMarkDownMsg(json &&rawMsg)
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

    CardMsg::CardMsg(json &&rawMsg)
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

    ItemMsg::ItemMsg(json &&rawMsg)
    {
        rawMessage = std::make_unique<json>(rawMsg);

        type = static_cast<Msg_Type>(rawMessage->at("type").get<int>());
    }

    SystemMsg::SystemMsg(json &&rawMsg)
    {
        rawMessage = std::make_unique<json>(rawMsg);

        type = rawMessage->at("type").get<string>();
    }
}