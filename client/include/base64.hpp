#pragma once
#include "stdafx.hpp"

///-------------------------------------------------------------------------------------------------
/// Base64 wrapper class for the crypto++ library.
///
/// @author Michael
/// @date   19.01.2017
///-------------------------------------------------------------------------------------------------

class base64
{
public:
    ///-------------------------------------------------------------------------------------------------
    /// URL encode.
    ///
    /// @author tominko (https://stackoverflow.com/a/4823686)
    /// @date   19.01.2017
    ///
    /// @param  input   The input.
    ///
    /// @return A std::string.
    ///-------------------------------------------------------------------------------------------------
    static std::string url_encode( const std::string& input );

    ///-------------------------------------------------------------------------------------------------
    /// Encodes.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   19.01.2017
    ///
    /// @param  plain           The plain.
    /// @param  encode_for_url  (Optional) URL of the encode for.
    ///
    /// @return A std::string.
    ///-------------------------------------------------------------------------------------------------
    static std::string encode( const std::string& plain,
                               const bool         encode_for_url = false );

    ///-------------------------------------------------------------------------------------------------
    /// Decodes the given encoded.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   19.01.2017
    ///
    /// @param  encoded The encoded.
    ///
    /// @return A std::string.
    ///-------------------------------------------------------------------------------------------------
    static std::string decode( const std::string& encoded );
};
