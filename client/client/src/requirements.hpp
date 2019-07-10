#pragma once
#if !defined(_CRT_SECURE_NO_WARNINGS)
    #define _CRT_SECURE_NO_WARNINGS
#endif

#if !defined(WIN32_LEAN_AND_MEAN)
    #define WIN32_LEAN_AND_MEAN
#endif

#if !defined(SODIUM_STATIC)
    #define SODIUM_STATIC 1
    #include <sodium.h>
#endif

#if !defined(no_discard)
    #define no_discard [[nodiscard]]
#endif

#include <array>
#include <string>
#include <string_view>
#include <sstream>
#include <vector>
#include <json.hpp>

#if !defined(CURL_STATICLIB)
    #define CURL_STATICLIB
    #include <curl/curl.h>
    #if defined(_DEBUG)
        #pragma comment(lib, "libcurl_a_debug.lib")
    #else
        #pragma comment(lib, "libcurl_a.lib")
    #endif
    #pragma comment(lib, "ws2_32.lib")
    #pragma comment(lib, "crypt32.lib")
    #pragma comment(lib, "wldap32.lib")
    #pragma comment(lib, "normaliz.lib")
#endif
