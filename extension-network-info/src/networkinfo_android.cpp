#if defined(DM_PLATFORM_ANDROID)

#include "networkinfo_private.h"
#include <dmsdk/dlib/android.h>

struct NetworkInfoJNI
{
    jobject        m_JNI;
    jmethodID      m_GetProxyInfo;

};

static NetworkInfoJNI g_NetworkInfo;

static char* CallCharMethodChar(jobject instance, jmethodID method, const char* cstr)
{
    dmAndroid::ThreadAttacher threadAttacher;
    JNIEnv* env = threadAttacher.GetEnv();

    jstring jstr = env->NewStringUTF(cstr);
    jstring return_value = (jstring)env->CallObjectMethod(instance, method, jstr);
    env->DeleteLocalRef(jstr);
    if (!return_value)
    {
        return 0;
    }

    const char* result_cstr = env->GetStringUTFChars(return_value, 0);
    char* result_cstr_copy = strdup(result_cstr);
    env->ReleaseStringUTFChars(return_value, result_cstr);
    env->DeleteLocalRef(return_value);

    return result_cstr_copy;
}

void NetworkInfo_PlatformInitialize()
{
    dmAndroid::ThreadAttacher threadAttacher;
    JNIEnv* env = threadAttacher.GetEnv();
    jclass cls = dmAndroid::LoadClass(env, "com.defold.networkinfo.NetworkInfoJNI");

    jmethodID jni_constructor = env->GetMethodID(cls, "<init>", "()V");
    g_NetworkInfo.m_JNI = env->NewGlobalRef(env->NewObject(cls, jni_constructor, threadAttacher.GetActivity()->clazz));
    g_NetworkInfo.m_GetProxyInfo = env->GetMethodID(cls, "GetProxyInfo", "(Ljava/lang/String;)Ljava/lang/String;");
}

void NetworkInfo_PlatformGetProxyInfo(const char* url, lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    const char* pack_name = luaL_checkstring(L, 1);
    char* json = CallCharMethodChar(g_NetworkInfo.m_JNI, g_NetworkInfo.m_GetProxyInfo, url);
    if (json)
    {
        dmScript::JsonToLua(L, json, strlen(json));
        free(json);
    }
    else
    {
        lua_pushnil(L);
    }
}

#endif
