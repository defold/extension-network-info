#if defined(DM_PLATFORM_ANDROID) || defined(DM_PLATFORM_IOS) || defined(DM_PLATFORM_MACOS)

#pragma once

#include <dmsdk/sdk.h>

void NetworkInfo_PlatformInitialize();
void NetworkInfo_PlatformGetProxyInfo(const char* url, lua_State* L);


#endif
