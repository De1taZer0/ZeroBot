#include "Setting.hh"

namespace ZeroBot::Setting
{
    auto initSetting() -> std::tuple<string, string, int>
    {
        path settingPath("./settings/settings.json");

        std::fstream settingFile(settingPath, std::ios::in);
        if(settingFile.fail())
        {
            create_directories(settingPath.parent_path());

            settingFile.open(settingPath, std::ios::out);
            if(settingFile.fail())
            {
                std::cerr << "File cannot be created." << std::endl;

                assert(settingFile.fail());
            }
            else
            {
                auto newSett = new json( { { "Authorization", "" }, { "Gateway", "" }, { "Compress", 0 } } );
                settingFile << newSett->dump(4);
                delete newSett;
            }
        }
        settingFile.close();

        settingFile.open(settingPath, std::ios::in);

        json settings;
        settingFile >> settings;
        if(settings["Authorization"].is_string() && settings["Gateway"].is_string() && settings["Compress"].is_number_unsigned())
        {
            if(settings["Authorization"].empty() || settings["Gateway"].empty() || settings["Compress"].empty()) // 未指定参数
            {
                // error
            }
            else
            {
                return std::make_tuple( settings.at("Authorization").get<string>(),
                                        settings.at("Gateway").get<string>(),
                                        settings.at("Compress").get<int>() );
            }
        }
        else // 参数错误
        {
            // error
        }
    }
}