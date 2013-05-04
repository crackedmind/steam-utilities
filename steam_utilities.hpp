#pragma once
#include <QString>

namespace SteamUtilities {
    QString GetSteamPath();
    QString GetSteamConfigFile();
    QString GetGameInstallPath(const char * game_id);
}
