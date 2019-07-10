#include "http.hpp"
using namespace ls;

std::string ls::http_get(
    const std::string_view& server,
    const std::string_view& file,
    const std::string_view& data,
    const long              timeout )
{
    const auto curl_data = curl_easy_init();
    if( !curl_data ) {
        return {};
    }

    auto query = std::string( server );
    if( !file.empty() ) {
        if( query.back() != '/' || file.front() != '/' ) {
            query += '/';
        }
        query.append( file );
    }
    if( !data.empty() ) {
        if( data.front() != '?' || query.back() != '?' ) {
            query += '?';
        }
        query.append( data );
    }

    std::string response;
    curl_easy_setopt( curl_data, CURLOPT_URL, query.c_str() );
    curl_easy_setopt( curl_data, CURLOPT_POST, 0L );
    curl_easy_setopt( curl_data, CURLOPT_TIMEOUT, timeout );
    curl_easy_setopt( curl_data, CURLOPT_SSL_VERIFYPEER, 1L );
    curl_easy_setopt( curl_data, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0 );
    curl_easy_setopt( curl_data, CURLOPT_MAXFILESIZE, 1024 * 1024 * 1024 );
    curl_easy_setopt( curl_data, CURLOPT_WRITEDATA, &response );
    curl_easy_setopt( curl_data, CURLOPT_WRITEFUNCTION, detail::curl_http_write_callback );

    const auto curl_response = curl_easy_perform( curl_data );
    curl_easy_cleanup( curl_data );

    return curl_response == CURLE_OK ? response : std::string();
}

std::string ls::http_post(
    const std::string_view& server,
    const std::string_view& file,
    const std::string_view& data,
    const long              timeout )
{
    const auto curl_data = curl_easy_init();
    if( !curl_data ) {
        return {};
    }

    auto query = std::string( server );
    if( !file.empty() ) {
        if( query.back() != '/' || file.front() != '/' ) {
            query += '/';
        }
        query.append( file );
    }

    std::string response;
    curl_easy_setopt( curl_data, CURLOPT_URL, query.c_str() );
    curl_easy_setopt( curl_data, CURLOPT_POST, 1L );
    curl_easy_setopt( curl_data, CURLOPT_POSTFIELDS, data.data() );
    curl_easy_setopt( curl_data, CURLOPT_TIMEOUT, timeout );
    curl_easy_setopt( curl_data, CURLOPT_SSL_VERIFYPEER, 1L );
    curl_easy_setopt( curl_data, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0 );
    curl_easy_setopt( curl_data, CURLOPT_MAXFILESIZE, 1024 * 1024 * 1024 );
    curl_easy_setopt( curl_data, CURLOPT_WRITEDATA, &response );
    curl_easy_setopt( curl_data, CURLOPT_WRITEFUNCTION, detail::curl_http_write_callback );

    const auto curl_response = curl_easy_perform( curl_data );
    curl_easy_cleanup( curl_data );

    return curl_response == CURLE_OK ? response : std::string();
}
