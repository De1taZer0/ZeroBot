#include "Setting.hh"

namespace ZeroBot::Setting
{
    void initSetting(json& settings)
    {
        path settingPath("./settings/settings.json");
        std::fstream settingFile(settingPath, std::ios::in);

        if(settingFile.fail())
        {
            create_directories(settingPath.parent_path());

            settingFile.open(settingPath, std::ios::in);
            if(settingFile.fail())
            {
                std::cerr << "File cannot be created." << std::endl;

                assert(settingFile.fail());
            }
            else
            {
                settingFile.close();
                settingFile.open(settingPath, std::ios::out);

                auto newSett = new json( { { "Authorization", "" }, { "Gateway", "" } } );
                settingFile << newSett->dump(4);
                delete newSett;
            }
        }
        settingFile.close();
        settingFile.open(settingPath, std::ios::in);

        settingFile >> settings;
        if(settings["Authorization"].is_string() && settings["Gateway"].is_string())
        {
            if(settings["Authorization"].empty() || settings["Gateway"].empty()) // 未指定参数
            {
                // error
            }
            else
            {
                return;
            }
        }
        else // 参数错误
        {
            // error
        }
    }
}