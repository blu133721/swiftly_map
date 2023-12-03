#include <swiftly/swiftly.h>
#include <swiftly/server.h>
#include <swiftly/commands.h>
#include <swiftly/configuration.h>
#include <swiftly/timers.h>

Server *server = nullptr;
PlayerManager *g_playerManager = nullptr;
Commands *commands = nullptr;
Configuration *config = nullptr;
Timers *timers = nullptr;

void OnProgramLoad(const char *pluginName, const char *mainFilePath)
{
    Swiftly_Setup(pluginName, mainFilePath);

    server = new Server();
    g_playerManager = new PlayerManager();
    commands = new Commands(pluginName);
    config = new Configuration();
    timers = new Timers();
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

void Timer()
{
    print("There are %02d players on the server.\n", g_playerManager->GetPlayers());
}

unsigned long long timerid;

int elapsedTime = 5;

void TimerCallback() {
    elapsedTime--;  // decrement elapsedTime.
    if (elapsedTime == 0) {
        timers->DestroyTimer(timerid);
    }
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
        elapsedTime = 5;
        timerid = timers->RegisterTimer(1000, TimerCallback);  
        if (elapsedTime == 0) {
            player->SendMsg(HUD_PRINTTALK, "[1TAP] Changing to Map: %s", config->Fetch<const char*>(mapNames[mapIndex]));
            server->ExecuteCommand("changelevel %s", config->Fetch<const char*>(mapNames[mapIndex]));
        }
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