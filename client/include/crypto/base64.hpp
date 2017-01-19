#pragma once
#include "../stdafx.hpp"

namespace crypto {

///-------------------------------------------------------------------------------------------------
/// Base64 wrapper class for the crypto++ library.
///
/// @author Michael
/// @date   19.01.2017
///-------------------------------------------------------------------------------------------------
class base64
{
    using Binary_t = std::vector<uint8_t>;

public:
    ///-------------------------------------------------------------------------------------------------
    /// URL encode.
    ///
    /// @author xperroni (https://stackoverflow.com/a/17708801)
    /// @date   19.01.2017
    ///
    /// @param  input   The input.
    ///
    /// @return The url encoded base64 string.
    ///-------------------------------------------------------------------------------------------------
    static std::string url_encode( const std::string& input );

    ///-------------------------------------------------------------------------------------------------
    /// Encodes a string with base64.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   19.01.2017
    ///
    /// @param  plain           The plain.
    /// @param  encode_for_url  (Optional) URL of the encode for.
    ///
    /// @return The encoded string if succeeded, empty string otherwise.
    ///-------------------------------------------------------------------------------------------------
    static std::string encode( const std::string& plain,
                               const bool         encode_for_url = false );

    ///-------------------------------------------------------------------------------------------------
    /// Encodes data with base64.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   19.01.2017
    ///
    /// @param  data            The data.
    /// @param  length          The length.
    /// @param  encode_for_url  (Optional) URL of the encode for.
    ///
    /// @return The encoded string if succeeded, empty string otherwise.
    ///-------------------------------------------------------------------------------------------------
    static std::string encode( const uint8_t* data,
                               const size_t length,
                               const bool encode_for_url = false );

    ///-------------------------------------------------------------------------------------------------
    /// Decodes the given encoded.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   19.01.2017
    ///
    /// @param  encoded The encoded.
    ///
    /// @return The encoded string if succeeded, empty string otherwise.
    ///-------------------------------------------------------------------------------------------------
    static std::string decode( const std::string& encoded );

    ///-------------------------------------------------------------------------------------------------
    /// Decode binary.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   19.01.2017
    ///
    /// @param  encoded The encoded.
    ///
    /// @return The encoded data if succeeded, empty data otherwise.
    ///-------------------------------------------------------------------------------------------------
    static Binary_t    decode_binary( const std::string& encoded );
};

} /// namespace crypto
