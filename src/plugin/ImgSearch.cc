#include "plugin/ImageSearch.hh"

inline std::string to_utf8(std::wstring& wide_string)
{
    static std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv;
    return utf8_conv.to_bytes(wide_string);
}

namespace ZeroBot::Plugin
{
    auto PluginImageSearch::update(const EventBase& msg) -> bool
    {
        if(msg.getType().mType != Msg_Type::SYSTEM && msg.content.find("/search") == 0)
        {
            imageSearchSign[msg.author_id] = true;
        }
        if(msg.getType().mType != Msg_Type::SYSTEM && imageSearchSign.contains(msg.author_id))
        {
            if(imageSearchSign.at(msg.author_id))
            {
                if(msg.content.find("https") != string::npos)
                {
                    std::thread thr([](const string& imgUrl, const string& targetID)
                    {
                        try
                        {
                            string command = "python ..\\addons\\searchImage.py --url=" + imgUrl;

                            FILE* fp = NULL;
                            wchar_t result[16384];
                            if((fp = _popen(command.c_str(), "r")) != NULL)
                            {
                                fgetws(result, 16384, fp);
                                _pclose(fp);
                                fp = NULL;
                            }

                            std::wstring wRes(result);
                            json resp = json::parse(to_utf8(wRes));

                            json resMsg = json::array();
                            resMsg[0]["type"] = "card";
                            resMsg[0]["theme"] = "secondary";
                            resMsg[0]["size"] = "lg";
                            resMsg[0]["modules"] = json::array();
                            resMsg[0]["modules"][0]["type"] = "section";
                            resMsg[0]["modules"][0]["text"]["type"] = "kmarkdown";
                            resMsg[0]["modules"][0]["text"]["content"] = "**找到以下结果(top 6):**";
                            for(int i = 0; i < 6; ++i)
                            {
                                int j = i * 2 + 1;
                                resMsg[0]["modules"][j]["type"] = "section";
                                resMsg[0]["modules"][j]["text"]["type"] = "kmarkdown";
                                resMsg[0]["modules"][j]["text"]["content"] = "---";

                                int k = i * 2 + 2;
                                resMsg[0]["modules"][k]["type"] = "section";
                                resMsg[0]["modules"][k]["text"]["type"] = "kmarkdown";
                                if(resp[i].contains("url"))
                                {
                                    resMsg[0]["modules"][k]["text"]["content"] = \
                                    "**标题: " + resp[i]["title"].get<string>() + "**\n" + \
                                    "**相似度: " + resp[i]["similarity"].get<string>() + "**\n" + \
                                    "**来自: " + resp[i]["index_name"].get<string>() + "**\n" + \
                                    "[点击跳转](" + resp[i]["url"].get<string>() + ")";
                                }
                                else
                                {
                                    resMsg[0]["modules"][k]["text"]["content"] = \
                                    "**标题: " + resp[i]["title"].get<string>() + "**\n" + \
                                    "**相似度: " + resp[i]["similarity"].get<string>() + "**\n" + \
                                    "**来自: " + resp[i]["index_name"].get<string>() + "**\n";
                                }
                                resMsg[0]["modules"][k]["mode"] = "right";
                                resMsg[0]["modules"][k]["accessory"]["type"] = "image";
                                resMsg[0]["modules"][k]["accessory"]["src"] = resp[i]["thumbnail"].get<string>();
                                resMsg[0]["modules"][k]["accessory"]["size"] = "lg";
                            }
                            Transmitter::sendGroupMsg(targetID, nlohmann::to_string(resMsg), std::optional(10));
                        }
                        catch (const std::exception& e)
                        {
                            std::cerr << e.what() << '\n';
                        }
                    }, msg.content, msg.target_id);

                    thr.detach();

                    imageSearchSign.at(msg.author_id) = false;
                }
                else
                {
                    Transmitter::sendGroupMsg(msg.target_id, "请发送图片");
                }
            }
        }
        return true;
    }
}