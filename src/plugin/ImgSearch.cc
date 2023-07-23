#include "plugin/ImageSearch.hh"

namespace ZeroBot::Plugin
{
    std::unordered_map<std::string, bool> PluginImageSearch::imageSearchSign;

    auto PluginImageSearch::update(const EventBase& msg) -> bool
    {
        if(msg.content.find("/search") == 0)
        {
            imageSearchSign[msg.author_id] = true;
        }
        if(imageSearchSign.contains(msg.author_id))
        {
            if(imageSearchSign.at(msg.author_id))
            {
                if(msg.content.find("https") != string::npos)
                {
                    try
                    {
                        string url = "https://saucenao.com";

                        httplib::SSLClient cli(url);

                        cli.set_ca_cert_path("../certs/ca.crt");

                        std::clog << 1 << std::endl;
                        httplib::Request req;
                        req.params = { { "api_key"    , apiKey      },
                                       { "dbmask"     , "999"       },
                                       { "output_type", "2"         },
                                       { "numres"     , "3"         },
                                       { "url"        , msg.content } };
                        req.method = "POST";
                        req.path = "/search.php";

                        httplib::Response resp;
                        httplib::Error err;

                        std::clog << 2 << std::endl;

                        cli.send(req, resp, err);
                        std::clog << 3 << std::endl;

                        std::cout << to_string(err) << std::endl;
                        std::cout << resp.body << std::endl;
                        Transmitter::sendGroupMsg(msg.target_id, resp.body);
                        std::clog << 4 << std::endl;
                    }
                    catch (const std::exception& e)
                    {
                        std::cerr << e.what() << '\n';
                    }
                    imageSearchSign.at(msg.author_id) = false;
                }
                else
                {
                    Transmitter::sendGroupMsg(msg.target_id, "请发送图片");
                }
            }
        }
    }
}