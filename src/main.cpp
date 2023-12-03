#include <swiftly/swiftly.h>
#include <swiftly/server.h>
#include <swiftly/database.h>
#include <swiftly/commands.h>
#include <swiftly/configuration.h>
#include <swiftly/logger.h>
#include <swiftly/timers.h>

Server *server = nullptr;
PlayerManager *g_playerManager = nullptr;
Database *db = nullptr;
Commands *commands = nullptr;
Configuration *config = nullptr;
Logger *logger = nullptr;
Timers *timers = nullptr;

void OnProgramLoad(const char *pluginName, const char *mainFilePath)
{
    Swiftly_Setup(pluginName, mainFilePath);

    server = new Server();
    g_playerManager = new PlayerManager();
    commands = new Commands(pluginName);
    config = new Configuration();
    logger = new Logger(mainFilePath, pluginName);
    timers = new Timers();
}

void Command_Map(int playerID, const char **args, uint32_t argsCount, bool silent)
{

        Player *player = g_playerManager->GetPlayer(playerID);
        if (!player)
            return;

        player->SendMsg(HUD_PRINTTALK, "[1TAP] -----------------------------------------------------");
        player->SendMsg(HUD_PRINTTALK, "!1 - Map: %s ", config->Fetch<const char*>("maps.map1"));
        player->SendMsg(HUD_PRINTTALK, "!2 - Map: %s ", config->Fetch<const char*>("maps.map2"));
        player->SendMsg(HUD_PRINTTALK, "!3 - Map: %s ", config->Fetch<const char*>("maps.map3"));
        player->SendMsg(HUD_PRINTTALK, "!4 - Map: %s ", config->Fetch<const char*>("maps.map4"));
        player->SendMsg(HUD_PRINTTALK, "!5 - Map: %s ", config->Fetch<const char*>("maps.map5"));
        player->SendMsg(HUD_PRINTTALK, "!6 - Map: %s ", config->Fetch<const char*>("maps.map6"));
        player->SendMsg(HUD_PRINTTALK, "!7 - Map: %s ", config->Fetch<const char*>("maps.map7"));
        player->SendMsg(HUD_PRINTTALK, "[1TAP] -----------------------------------------------------");
        return;
}

void Command_Map1(int playerID, const char **args, uint32_t argsCount, bool silent)
{
        server->ExecuteCommand("changelevel %s", config->Fetch<const char*>("maps.map1"));
        return;
}

void OnPluginStart()
{
    commands->Register("map", reinterpret_cast<void *>(&Command_Map));
    commands->Register("1", reinterpret_cast<void *>(&Command_Map1));
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