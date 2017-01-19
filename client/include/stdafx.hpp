#pragma once
/// Enable this define as far as you link the client static
#define CLIENT_STATIC_LIB

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

#if defined(_MSC_VER) && defined(CLIENT_STATIC_LIB)
/// crypto++ include files
#include "../contrib/cryptopp565/crypto++/cryptlib.h"
#include "../contrib/cryptopp565/crypto++/aes.h"
#include "../contrib/cryptopp565/crypto++/base64.h"
#include "../contrib/cryptopp565/crypto++/filters.h"
#include "../contrib/cryptopp565/crypto++/modes.h"
#include "../contrib/cryptopp565/crypto++/hex.h"
#include "../contrib/cryptopp565/crypto++/osrng.h"
#include "../contrib/cryptopp565/crypto++/rsa.h"
#include "../contrib/cryptopp565/crypto++/gcm.h"
/// nlohmann::json
#include <../contrib/nlohmann/json.hpp>
/// tfm::format, tfm::printf, tfm::printIn
#include <../contrib/tinyformat/tinyformat.hpp>
#else
/// crypto++ include files
#include <crypto++/cryptlib.h>
#include <crypto++/aes.h>
#include <crypto++/base64.h>
#include <crypto++/filters.h>
#include <crypto++/modes.h>
#include <crypto++/hex.h>
#include <crypto++/osrng.h>
#include <crypto++/rsa.h>
#include <crypto++/gcm.h>
/// nlohmann::json
#include <json.hpp>
/// tfm::format, tfm::printf, tfm::printIn
#include <tinyformat.hpp>

#endif
