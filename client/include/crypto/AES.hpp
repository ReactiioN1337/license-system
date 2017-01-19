#pragma once
#include "../stdafx.hpp"

namespace crypto {

///-------------------------------------------------------------------------------------------------
/// AES wrapper class for the crypto++ library.
///
/// @author ReactiioN (https://github.com/ReactiioN1337)
/// @date   19.01.2017
///-------------------------------------------------------------------------------------------------

class AES
{
public:
    ///-------------------------------------------------------------------------------------------------
    /// AES KeyPair (key(32 bytes) & iv(16 bytes).
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   19.01.2017
    ///-------------------------------------------------------------------------------------------------
    class KeyPair
    {
    public:
        ///-------------------------------------------------------------------------------------------------
        /// Default constructor.
        ///
        /// @author ReactiioN (https://github.com/ReactiioN1337)
        /// @date   19.01.2017
        ///-------------------------------------------------------------------------------------------------
        KeyPair();

        ///-------------------------------------------------------------------------------------------------
        /// Constructor.
        ///
        /// @author ReactiioN (https://github.com/ReactiioN1337)
        /// @date   19.01.2017
        ///
        /// @param  key The key as base64.
        /// @param  iv  The iv as base64.
        ///-------------------------------------------------------------------------------------------------
        KeyPair( const std::string& key, const std::string& iv );

        ///-------------------------------------------------------------------------------------------------
        /// Loads an iv.
        ///
        /// @param  iv  The iv as base64.
        ///
        /// @return True if it succeeds, false if it fails.
        ///-------------------------------------------------------------------------------------------------
        bool        load_iv( const std::string& iv );

        ///-------------------------------------------------------------------------------------------------
        /// Loads a key.
        ///
        /// @param  key The key as base64.
        ///
        /// @return True if it succeeds, false if it fails.
        ///-------------------------------------------------------------------------------------------------
        bool        load_key( const std::string& key );

        ///-------------------------------------------------------------------------------------------------
        /// Gets the key.
        ///
        /// @return The key as base64 string.
        ///-------------------------------------------------------------------------------------------------
        std::string get_key() const;

        ///-------------------------------------------------------------------------------------------------
        /// Gets the iv.
        ///
        /// @return The iv as base64 string.
        ///-------------------------------------------------------------------------------------------------
        std::string get_iv() const;

        /// Generates this object.
        void        generate();

    public:
        /// The key.
        CryptoPP::SecByteBlock key;

        /// The AES iv.
        CryptoPP::SecByteBlock iv;
    };

public:
    ///-------------------------------------------------------------------------------------------------
    /// Encrypts a string with AES.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   19.01.2017
    ///
    /// @param  plain   The plain.
    /// @param  key     The key.
    /// @param  iv      The iv.
    ///
    /// @return The cipher as base64 string if succeeded, empty string otherwise.
    ///-------------------------------------------------------------------------------------------------
    static std::string encrypt( const std::string& plain,
                                const std::string& key,
                                const std::string& iv );

    ///-------------------------------------------------------------------------------------------------
    /// Encrypts a string with AES.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   19.01.2017
    ///
    /// @param  plain       The plain.
    /// @param  key         The key.
    /// @param  key_size    Size of the key.
    /// @param  iv          The iv.
    /// @param  iv_size     Size of the iv.
    ///
    /// @return The cipher as base64 string if succeeded, empty string otherwise.
    ///-------------------------------------------------------------------------------------------------
    static std::string encrypt( const std::string& plain,
                                const uint8_t*     key,
                                const size_t       key_size,
                                const uint8_t*     iv,
                                const size_t       iv_size );

    ///-------------------------------------------------------------------------------------------------
    /// Encrypts a string with AES.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   19.01.2017
    ///
    /// @param          plain   The plain.
    /// @param [in,out] kp      The key pair.
    ///
    /// @return The cipher as base64 string if succeeded, empty string otherwise.
    ///-------------------------------------------------------------------------------------------------
    static std::string encrypt( const std::string& plain,
                                KeyPair&           kp );

    ///-------------------------------------------------------------------------------------------------
    /// Decrypts an AES cipher.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   19.01.2017
    ///
    /// @param  cipher  The cipher.
    /// @param  key     The key.
    /// @param  iv      The iv.
    ///
    /// @return The plain string if succeeded, empty string otherwise.
    ///-------------------------------------------------------------------------------------------------
    static std::string decrypt( const std::string& cipher,
                                const std::string& key,
                                const std::string& iv );

    ///-------------------------------------------------------------------------------------------------
    /// Decrypts an AES cipher.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   19.01.2017
    ///
    /// @param  cipher      The cipher.
    /// @param  key         The key.
    /// @param  key_size    Size of the key.
    /// @param  iv          The iv.
    /// @param  iv_size     Size of the iv.
    ///
    /// @return The plain string if succeeded, empty string otherwise.
    ///-------------------------------------------------------------------------------------------------
    static std::string decrypt( const std::string& cipher,
                                const uint8_t*     key,
                                const size_t       key_size,
                                const uint8_t*     iv,
                                const size_t       iv_size );

    ///-------------------------------------------------------------------------------------------------
    /// Decrypts an AES cipher.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   19.01.2017
    ///
    /// @param          cipher  The cipher.
    /// @param [in,out] kp      The kp.
    ///
    /// @return The plain string if succeeded, empty string otherwise.
    ///-------------------------------------------------------------------------------------------------
    static std::string decrypt( const std::string& cipher,
                                KeyPair&           kp );
    
};

}
