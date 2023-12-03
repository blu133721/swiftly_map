#include <swiftly/swiftly.h>
#include <swiftly/server.h>
#include <swiftly/commands.h>
#include <swiftly/configuration.h>

Server *server = nullptr;
PlayerManager *g_playerManager = nullptr;
Commands *commands = nullptr;
Configuration *config = nullptr;

void OnProgramLoad(const char *pluginName, const char *mainFilePath)
{
    Swiftly_Setup(pluginName, mainFilePath);

    server = new Server();
    g_playerManager = new PlayerManager();
    commands = new Commands(pluginName);
    config = new Configuration();
}

void Command_Maps(int playerID, const char **args, uint32_t argsCount, bool silent)
{
    Player *player = g_playerManager->GetPlayer(playerID);
    if (!player)
        return;

    player->SendMsg(HUD_PRINTTALK, "[1TAP] -----------------------------------------------------");

    const char* mapNames[] = {
        "maps.map1",
        "maps.map2",
        "maps.map3",
        "maps.map4",
        "maps.map5",
        "maps.map6",
        "maps.map7"
    };

    for (int i = 0; i < sizeof(mapNames) / sizeof(mapNames[0]); ++i)
    {
        player->SendMsg(HUD_PRINTTALK, "%d - Map: %s", i + 1, config->Fetch<const char*>(mapNames[i]));
    }

    player->SendMsg(HUD_PRINTTALK, "[1TAP] -----------------------------------------------------");
}

struct TimerData {
    int mapIndex;
    Player* player;
};

void Timer(int timerID, void* data)
{
    TimerData* timerData = static_cast<TimerData*>(data);
    server->ExecuteCommand("changelevel %s", config->Fetch<const char*>(mapNames[timerData->mapIndex]));
    timerData->player->SendMsg(HUD_PRINTTALK, "[1TAP] Changing to Map: %s", config->Fetch<const char*>(mapNames[timerData->mapIndex]));
    delete timerData;  // Don't forget to delete the data when you're done with it
}

void Command_Map(int playerID, const char **args, uint32_t argsCount, bool silent)
{
    Player *player = g_playerManager->GetPlayer(playerID);
    if (!player || argsCount < 1)
        return;

    const char *mapNumber = args[0];
    const char *mapNames[] = {
        "maps.map1",
        "maps.map2",
        "maps.map3",
        "maps.map4",
        "maps.map5",
        "maps.map6",
        "maps.map7"
    };

    int mapIndex = atoi(mapNumber) - 1;

    if (mapIndex >= 0 && mapIndex < sizeof(mapNames) / sizeof(mapNames[0]))
    {
        TimerData* data = new TimerData{mapIndex, player};
        timers->RegisterTimer(5000, Timer, data);
    }
    else
    {
        player->SendMsg(HUD_PRINTTALK, "[1TAP] Invalid map number. Use a number between 1 and 7.");
    }
}

void OnPluginStart()
{
    commands->Register("map", reinterpret_cast<void*>(&Command_Map));
    commands->Register("maps", reinterpret_cast<void*>(&Command_Maps));
}

void OnPluginStop()
{
}

const char *GetPluginAuthor()
{
    return "";
}

const char *GetPluginVersion()
{
    return "1.0.0";
}

const char *GetPluginName()
{
    return "swiftly_map";
}

const char *GetPluginWebsite()
{
    return "";
}