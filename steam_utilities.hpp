#pragma once
#include <QString>

namespace SteamUtilities {
    QString GetSteamPath();
    QString GetSteamConfigFile();
    QString GetGameInstallPath(const QLatin1String game_id);
}
