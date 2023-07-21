#include "Transmitter.hh"

namespace ZeroBot::Transmit
{
    auto Transmitter::sendHttpRequest(HttpRequestPtr req) -> json
    {
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
                return res;
            }

            rateLimit.limit = std::atoi(resp->GetHeader("X-Rate-Limit-Limit", "0").c_str());
            rateLimit.remaining = std::atoi(resp->GetHeader("X-Rate-Limit-Remaining", "0").c_str());
            rateLimit.reset = std::atoi(resp->GetHeader("X-Rate-Limit-Reset", "0").c_str());
            rateLimit.bucket = std::atoi(resp->GetHeader("X-Rate-Limit-Bucket", "0").c_str());
            rateLimit.global = std::atoi(resp->GetHeader("X-Rate-Limit-Global", "0").c_str());

            return res;
        }
        catch(const std::exception& e)
        {
            std::cerr << "Error occurred when sending HttpRequest: " << e.what();
            throw e;
        }
    }

    auto Transmitter::init() -> void
    {
        authorization = std::get<0>(Setting::initSetting());
    }

    auto Transmitter::sendGroupMsg(crstring target_id, crstring content, ocrint type, ocrstring quote, ocrstring temp_target_id) -> optional<std::pair<string, int>>
    {
        HttpRequestPtr req(new HttpRequest);

        req->method = HTTP_POST;
        req->url = "https://www.kookapp.cn/api/v3/message/create";
        req->json = { { "target_id"     , target_id      },
                      { "content"       , content        } };

        if(type.has_value())
        {
            req->json["type"] = type.value().get();
        }
        if(quote.has_value())
        {
            req->json["quote"] = quote.value().get();
        }
        if(temp_target_id != std::nullopt)
        {
            req->json["temp_target_id"] = temp_target_id.value().get();
        }

        try
        {
            auto res = sendHttpRequest(req);

            return std::make_pair(res.at("data").at("msg_id").get<string>(), res.at("data").at("msg_timestamp").get<int>());
        }
        catch(const std::exception& e)
        {
            std::cerr << "\tIn Transmitter::sendGroupMsg: " << e.what() << std::endl;
            return std::nullopt;
        }
    }
}