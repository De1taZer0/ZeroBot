#ifndef ZEROBOT_SETTING_HH
#define ZEROBOT_SETTING_HH

///
/// @file Setting.hh
/// @date 2023.7.14
///

#include <tuple>
#include <string>
#include <cassert>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <hv/json.hpp>

namespace ZeroBot::Setting
{
    using std::filesystem::create_directories;
    using path = std::filesystem::path;
    using json = nlohmann::json;
    using string = std::string;

    ///
    /// \param settings
    std::tuple<string, string, int> initSetting();
}

#endif //ZEROBOT_SETTING_HH
