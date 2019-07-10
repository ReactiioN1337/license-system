#pragma once
#include "../requirements.hpp"

namespace ls::detail {

static std::size_t curl_http_write_callback(
    char*             ptr,
    const std::size_t size,
    const std::size_t nmemb,
    void*             userdata )
{
    auto* data = reinterpret_cast<std::string*>( userdata );
    data->append( ptr, size * nmemb );
    return size * nmemb;
}

}
