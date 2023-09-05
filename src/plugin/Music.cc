#include "plugin/Music.hh"

#define MusicSelectExpiration (20000)

namespace ZeroBot::Plugin
{
    auto PluginMusic::update(const EventBase& msg) -> bool
    {
        if(msg.getType().mType != Msg_Type::SYSTEM && msg.content.find("/music") == 0)
        {
            std::thread thr([](const string& searchMsg, const string& targetID, const time_t& timeSt)
            {
                try
                {
                    string url = "https://xiaoapi.cn/API/yy_sq.php?msg=" + searchMsg;
                    auto resp = requests::get(url.c_str());

                    json respJson = resp->GetJson()["list"];
                    json resMsg = json::array();
                    resMsg[0]["type"] = "card";
                    resMsg[0]["theme"] = "primary";
                    resMsg[0]["size"] = "lg";
                    resMsg[0]["color"] = "#00FFFF";
                    resMsg[0]["modules"] = nlohmann::ordered_json::array();
                    resMsg[0]["modules"].push_back(json{ { "type"     , "countdown"                    },
                                                         { "mode"     , "second"                       },
                                                         { "startTime", timeSt                         },
                                                         { "endTime"  , timeSt + MusicSelectExpiration } });
                    int j = 1;
                    for(auto& i : respJson)
                    {
                        resMsg[0]["modules"].push_back(json{ { "type", "section"                          },
                                                             { "text", json{ { "type"   , "kmarkdown" },
                                                                             { "content", "---"       } } } });

                        string tempContent = "\t**" + i.at("name").get<string>() + "**\n\t" + i.at("singer").get<string>();
                        resMsg[0]["modules"].push_back(json{ { "type"     , "section"                                                 },
                                                             { "text"     , json{ { "type"   , "kmarkdown"                        },
                                                                                  { "content", tempContent                        } } },
                                                             { "mode"     , "right"                                                   },
                                                             { "accessory", json{ { "type"   , "button"                           },
                                                                                  { "theme"  , "info"                             },
                                                                                  { "value"  , std::to_string(j)                  },
                                                                                  { "click"  , "return-val"                       },
                                                                                  { "text"   , json{ { "type"   , "kmarkdown" },
                                                                                                     { "content", "**点歌**"   } } } } } });
                        ++j;
                    }
                    auto resu =  Transmitter::sendGroupMsg(targetID, nlohmann::to_string(resMsg), std::optional(10));
                    std::unique_lock tLock(mapMutex);
                    musicSelectCard[resu.value().first] = url;
                    tLock.unlock();

                    hv_msleep(MusicSelectExpiration);

                    tLock.lock();
                    musicSelectCard.erase(resu.value().first);
                    tLock.unlock();
                }
                catch(const std::exception& e)
                {
                    std::cout << "Error in PluginMusic::update1: " << e.what() << std::endl;
                }
            }, msg.content.substr(7,msg.content.length() - 7), msg.target_id, msg.msg_timestamp);
            thr.detach();
        }
        else if(msg.getType().mType == Msg_Type::SYSTEM)
        {
            if(msg.extra->rawMessage->at("type").get<string>() == "message_btn_click")
            {
                json tempJson(*(msg.extra->rawMessage));
                std::thread thr([](const json& tJson)
                {
                    try
                    {
                        auto msg_id = tJson.at("body").at("msg_id").get<string>();
                        auto user_id = tJson.at("body").at("user_id").get<string>();
                        auto value = tJson.at("body").at("value").get<string>();
                        auto target_id = tJson.at("body").at("target_id").get<string>();
                        std::shared_lock tLock(mapMutex);
                        if(musicSelectCard.contains(msg_id))
                        {
                            string url = musicSelectCard.at(msg_id) + "&n=" + value;
                            tLock.unlock();
                            auto resp = requests::get(url.c_str());
                            auto respJson = resp->GetJson();
                            auto resMsg = json::array();
                            resMsg[0] = json{ { "type"   , "card"        },
                                              { "theme"  , "secondary"   },
                                              { "sizze"  , "lg"          },
                                              { "modules", json::array() } };

                            auto resMusic = requests::get(respJson.at("url").get<string>().c_str());
                            char rand[20];
                            hv_random_string(rand, 18);
                            string randName(rand);
                            string flacPath = "./temp/music/" + randName + ".flac";
                            string mp3Path = "./temp/music/" + randName + ".mp3";
                            std::ofstream musicOutput(flacPath, std::ios::binary);
                            musicOutput.write(resMusic->body.c_str(), resMusic->content_length);
                            musicOutput.close();

                            string command = "ffmpeg -i " + flacPath + " -ab 320k -map_metadata 0 -id3v2_version 3 " + mp3Path;
                            FILE* fp = NULL;
                            if((fp = _popen(command.c_str(), "r")) != NULL)
                            {
                                _pclose(fp);
                                fp = NULL;
                            }
                            remove(flacPath.c_str());

                            std::ifstream mp3File(mp3Path, std::ios::binary);
                            string mp3Bin((std::istreambuf_iterator<char>(mp3File)), std::istreambuf_iterator<char>());
                            mp3File.close();
                            remove(mp3Path.c_str());

                            HttpRequestPtr uploadReq(new HttpRequest);
                            uploadReq->url = "https://www.kookapp.cn/api/v3/asset/create";
                            uploadReq->method = HTTP_POST;
                            uploadReq->headers["Authorization"] = std::get<0>(Setting::initSetting());
                            uploadReq->headers["Content-Type"] = "multipart/form-data";
                            uploadReq->form["file"].filename = randName + ".mp3";
                            uploadReq->form["file"].content = mp3Bin;

                            HttpResponsePtr uploadResp(new HttpResponse);
                            auto errcode = http_client_send(uploadReq.get(), uploadResp.get());
                            if(errcode != 0)
                            {
                                std::cerr << "Upload fail." << std::endl;
                            }
                            std::cerr << uploadResp->Dump(true, true) << std::endl;

                            resMsg[0]["modules"][0] = json{ { "type" , "audio"                                                  },
                                                            { "title", respJson.at("name")                                      },
                                                            { "src"  , uploadResp->GetJson().at("data").at("url").get<string>() },
                                                            { "cover", respJson.at("cover")                                     } };

                            Transmitter::sendGroupMsg(target_id, nlohmann::to_string(resMsg), std::optional(10));


                        }
                        else
                        {
                            tLock.unlock();
                        }
                    }
                    catch(const std::exception& e)
                    {
                        std::cout << "Error in PluginMusic::update2: " << e.what() << std::endl;
                    }
                }, tempJson);
                thr.detach();
            }
        }
        return 1;
    }
}