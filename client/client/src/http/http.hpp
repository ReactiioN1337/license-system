#pragma once
#include "write_callbacks.hpp"

namespace ls {

extern std::string http_get(
    const std::string_view& server,
    const std::string_view& file,
    const std::string_view& data,
    long                    timeout = 5 );

extern std::string http_post(
    const std::string_view& server,
    const std::string_view& file,
    const std::string_view& data,
    long                    timeout = 5 );

}
