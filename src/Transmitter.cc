#include "Transmitter.hh"

namespace ZeroBot::Transmit
{
    auto Transmitter::init() -> void
    {
        authorization = std::get<0>(Setting::initSetting());
    }

    auto Transmitter::sendGroupMsg(const string &target_id, const string &content) -> std::pair<string, int>
    {
        HttpRequestPtr req(new HttpRequest);

        req->method = HTTP_POST;
        req->url = "https://www.kookapp.cn/api/v3/message/create";
        req->headers["Authorization"] = authorization;
        req->json = { { "target_id" , target_id },
                      { "content"   , content   } };

        try
        {
            HttpResponsePtr resp(new HttpResponse);

            http_client_send(req.get(), resp.get());

            json res = resp->GetJson();

            std::cout << res << std::endl;
            if(res.at("code").get<int>() != 0)
            {
                std::cout << req->body << std::endl;
                std::cerr << "Send GroupMsg failed. Error code: " << res.at("code").get<int>() << std::endl;
                return std::make_pair("" , 0);
            }

            std::cout << std::atoi(resp->GetHeader("X-Rate-Limit-Limit", "0").c_str()) << std::endl;
            std::cout << std::atoi(resp->GetHeader("X-Rate-Limit-Remaining", "0").c_str()) << std::endl;
            std::cout << std::atoi(resp->GetHeader("X-Rate-Limit-Reset", "0").c_str()) << std::endl;
            std::cout << std::atoi(resp->GetHeader("X-Rate-Limit-Bucket", "0").c_str()) << std::endl;
            std::cout << std::atoi(resp->GetHeader("X-Rate-Limit-Global", "0").c_str()) << std::endl;

            return std::make_pair(res.at("data").at("msg_id").get<string>(), res.at("data").at("msg_timestamp").get<int>());
        }
        catch(const std::exception& e)
        {
            std::cerr << "Error occurred when sending GroupMsg: " << req->body;
            std::cerr << "\n\t" << e.what();
        }
    }
}