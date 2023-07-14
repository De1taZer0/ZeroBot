#ifndef ZEROBOT_SETTING_HH
#define ZEROBOT_SETTING_HH

///
/// @file ZeroBot.hh
/// @date 2023.7.14
///

#include <cassert>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <hv/json.hpp>

namespace ZeroBot::Setting
{
    using std::filesystem::path;
    using std::filesystem::create_directories;
    using nlohmann::json;

    ///
    /// \param settings
    void initSetting(json& settings);
}

#endif //ZEROBOT_SETTING_HH
