#pragma once
#ifdef _MSC_VER
/// Otherwise min/max in json won't work without a minimal modification.
#define NOMINMAX
#endif

#ifndef WEB_ENCODER
/// Windows socket
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment( lib, "Ws2_32.lib" )
/// WindowsAPI
#include <Windows.h>
#endif

/// std::array
#include <array>
/// int64_t
#include <cstdint>
/// std::map
#include <map>
/// std::stringstream
#include <sstream>
/// std::string
#include <string>
/// std::vector
#include <vector>
/// nlohmann::json
#include <json.hpp>
/// tfm::format, tfm::printf, tfm::printIn
#include <tinyformat.hpp>

/// crypto++ include files
#include <crypto++/cryptlib.h>
#include <crypto++/aes.h>
#include <crypto++/base64.h>
#include <crypto++/filters.h>
#include <crypto++/aes.h>
#include <crypto++/modes.h>
#include <crypto++/hex.h>
#include <crypto++/osrng.h>
#include <crypto++/rsa.h>
#include <crypto++/gcm.h>
