#include <QFile>
#include <QTextStream>
#include <Windows.h>
#include "steam_utilities.hpp"

const wchar_t STEAM_PATH[] = L"SteamPath";
const wchar_t STEAM_ROOT[] = L"Software\\Valve\\Steam";

// TODO: more error handling

// TODO: linux version
QString SteamUtilities::GetSteamPath()
{
    wchar_t *data = NULL;
    unsigned long type = REG_SZ;
    unsigned long size = 0;

    QString res;

    HKEY steam_root;

    if ( RegOpenKey(HKEY_CURRENT_USER, STEAM_ROOT, &steam_root) == ERROR_SUCCESS )
    {
        if ( RegQueryValueEx(steam_root, STEAM_PATH, NULL, &type, NULL,  &size) == ERROR_SUCCESS )
        {
            data = new wchar_t[size];

            if ( RegQueryValueEx(steam_root, STEAM_PATH, NULL, &type, LPBYTE(data), &size) == ERROR_SUCCESS )
            {
               res = QString::fromWCharArray(data);
            }
        }
    }
    delete [] data;
    return res;
}

QString SteamUtilities::GetSteamConfigFile()
{
    QString steam_path = SteamUtilities::GetSteamPath();

    if ( !steam_path.isEmpty() )
    {
        return QString("%1/config/config.vdf").arg(steam_path);
    }
    return QString();
}

// very very dirty function
// because i'm too lazy to write full-fledged adf/vdf parser
// return empty string if game wasn't found
QString SteamUtilities::GetGameInstallPath(const QLatin1String game_id)
{
    QFile config(SteamUtilities::GetSteamConfigFile());

    if ( config.open(QFile::Text | QFile::ReadOnly) )
    {
        QTextStream input(&config);
        QString lines = input.readAll();

        int game_root = lines.indexOf(game_id);

        if ( game_root != -1 )
        {
            int block_start = lines.indexOf('{', game_root);
            int block_end = lines.indexOf('}', game_root);

            // with +1 and -1 skipping '{' and '}' chars
            QString vdf_block = lines.mid(block_start + 1, block_end - block_start - 1);

            vdf_block.replace('\n',"");
            vdf_block.replace('\"',"");

            QStringList vdf_options = vdf_block.trimmed().split('\t', QString::SkipEmptyParts);

            for ( int i = 0; i < vdf_options.size();i++)
            {
                if ( vdf_options[i] == QLatin1String("installdir") )
                    return vdf_options[i+1];
            }
        }
    }

    return QString();
}
