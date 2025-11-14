#if defined(DM_PLATFORM_IOS) || defined(DM_PLATFORM_MACOS)

#include "networkinfo_private.h"
#include <dmsdk/sdk.h>
#import <Foundation/Foundation.h>

void NetworkInfo_PlatformInitialize() {}

void NetworkInfo_PlatformGetProxyInfo(const char* url, lua_State* L)
{
    dmLogInfo("NetworkInfo_PlatformGetProxyInfo");
    CFDictionaryRef systemProxySettings = CFNetworkCopySystemProxySettings();

    CFURLRef urlref = CFURLCreateWithBytes(kCFAllocatorDefault, (const UInt8*)url, strlen(url), kCFStringEncodingUTF8, 0);

    NSArray* proxies = [(NSArray *) CFNetworkCopyProxiesForURL(urlref, systemProxySettings) autorelease];

    lua_newtable(L);

    int proxy_count = proxies.count;
    if (proxy_count == 0)
    {
        dmLogInfo("NetworkInfo_PlatformGetProxyInfo no proxies");
        return;
    }

    for (int i = 0; i < proxy_count; i++)
    {
        lua_pushnumber(L, i + 1);
        lua_newtable(L);

        NSDictionary* proxySettings = [proxies objectAtIndex:i];

        lua_pushstring(L, "type");
        NSString* type = (NSString*)[proxySettings objectForKey:(NSString*)kCFProxyTypeKey];
        if ((type == (NSString*)kCFProxyTypeHTTP) || (type == (NSString*)kCFProxyTypeHTTPS)) {
            lua_pushstring(L, "http");
        }
        else if (type == (NSString*)kCFProxyTypeSOCKS) {
            lua_pushstring(L, "socks");
        }
        else if (type == (NSString*)kCFProxyTypeNone) {
            lua_pushstring(L, "direct");
        }
        else {
            lua_pushstring(L, "unknown");
        }
        lua_rawset(L, -3);

        if ([proxySettings objectForKey:(NSString*)kCFProxyPortNumberKey]) {
            lua_pushstring(L, "port");
            lua_pushnumber(L, [[proxySettings objectForKey:(NSString*)kCFProxyPortNumberKey] floatValue]);
            lua_rawset(L, -3);
        }
        if ([proxySettings objectForKey:(NSString*)kCFProxyHostNameKey]) {
            lua_pushstring(L, "host");
            lua_pushstring(L, [[proxySettings objectForKey:(NSString*)kCFProxyHostNameKey] UTF8String]);
            lua_rawset(L, -3);
        }

        lua_rawset(L, -3);
    }
    

}

#endif
