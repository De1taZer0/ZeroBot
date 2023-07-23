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
                        std::unique_ptr<hv::HttpClient> cli(new hv::HttpClient);

                        hssl_ctx_opt_t *ssl_opt = new hssl_ctx_opt_t;
                        ssl_opt->verify_peer = 0;
                        ssl_opt->endpoint = HSSL_CLIENT;
                        ssl_opt->ca_path = NULL;
                        ssl_opt->ca_file = "../cert/ca.crt";
                        ssl_opt->crt_file = "../cert/client.crt";
                        ssl_opt->key_file = "../cert/client_rsa_private.pem";
                        hssl_ctx_t ctx = hssl_ctx_new(ssl_opt);
                        if(ctx == NULL)
                        {
                            std::cerr << "Error: ctx is null" << std::endl;
                        }
                        delete ssl_opt;
                        auto ret = cli->setSslCtx(ctx);
                        if (ret != 0)
                        {
                            std::cerr << "Error: Cert Error" << http_client_strerror(ret) << std::endl;
                        }

                        HttpRequestPtr req(new HttpRequest);
                        req->method = HTTP_POST;
                        req->url = "https://saucenao.com";
                        req->path = "/search.php?api_key=" + apiKey + "&dbmask=999&output_type=2&numres=3";

                        HttpResponsePtr resp(new HttpResponse);

                        cli->send(req.get(), resp.get());

                        std::cout << resp->status_code << std::endl;
                        string str;
                        resp->DumpHeaders(str);
                        std::cout << str << std::endl;

                        Transmitter::sendGroupMsg(msg.target_id, nlohmann::to_string(resp->GetJson()));
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
        return true;
    }
}