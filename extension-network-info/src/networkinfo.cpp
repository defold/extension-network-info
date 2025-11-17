/** Network Info API
 * Functions to get information about the network configuration of the device.
 * @document
 * @name networkinfo
 * @namespace networkinfo
 */

#define EXTENSION_NAME NetworkInfo
#define LIB_NAME "NetworkInfo"
#define MODULE_NAME "networkinfo"

#define DLIB_LOG_DOMAIN LIB_NAME
#include <dmsdk/sdk.h>

#if defined(DM_PLATFORM_ANDROID) || defined(DM_PLATFORM_IOS) || defined(DM_PLATFORM_MACOS)

#include "networkinfo_private.h"


/** Get information about proxies for a specific url
 * @name get_proxy_info
 * @string url The url to get proxy configurations for
 * @treturn table proxies A list of proxies to try in order. Each proxy has three values
 *   type: direct (no proxy), http, socks
 *   host: proxy url
 *   port: proxy port number
 */

static int NetworkInfo_GetProxyInfo(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    const char* url = luaL_checkstring(L, 1);
    NetworkInfo_PlatformGetProxyInfo(url, L);
    return 1;
}

static const luaL_reg Module_methods[] =
{
    {"get_proxy_info", NetworkInfo_GetProxyInfo},
    {0, 0}
};

static void LuaInit(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 0);
    luaL_register(L, MODULE_NAME, Module_methods);
    lua_pop(L, 1);
}

static dmExtension::Result AppInitializeNetworkInfo(dmExtension::AppParams* params)
{
    return dmExtension::RESULT_OK;
}

static dmExtension::Result InitializeNetworkInfo(dmExtension::Params* params)
{
    LuaInit(params->m_L);
    NetworkInfo_PlatformInitialize();
    return dmExtension::RESULT_OK;
}

static dmExtension::Result AppFinalizeNetworkInfo(dmExtension::AppParams* params)
{
    return dmExtension::RESULT_OK;
}

static dmExtension::Result FinalizeNetworkInfo(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}

DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, AppInitializeNetworkInfo, AppFinalizeNetworkInfo, InitializeNetworkInfo, 0, 0, FinalizeNetworkInfo)

#else

static  dmExtension::Result InitializeNetworkInfo(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}

static dmExtension::Result FinalizeNetworkInfo(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}

DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, 0, 0, InitializeNetworkInfo, 0, 0, FinalizeNetworkInfo)

#endif // MacOS/IOS/Android
