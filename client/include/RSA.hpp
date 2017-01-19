#pragma once
#include "stdafx.hpp"

///-------------------------------------------------------------------------------------------------
/// RSA wrapper class for the crypto++ library.
///
/// @author ReactiioN (https://github.com/ReactiioN1337)
/// @date   19.01.2017
///-------------------------------------------------------------------------------------------------
class RSA
{
public:
    class KeyPair
    {
    public:
        ///-------------------------------------------------------------------------------------------------
        /// Default constructor.
        ///
        /// @author ReactiioN (https://github.com/ReactiioN1337)
        /// @date   19.01.2017
        ///-------------------------------------------------------------------------------------------------
        KeyPair() = default;

        ///-------------------------------------------------------------------------------------------------
        /// Loads the public key.
        ///
        /// @param  key         The key.
        /// @param  as_base64   (Optional) Is the key base64 encoded?
        ///
        /// @return True if it succeeds, false if it fails.
        ///-------------------------------------------------------------------------------------------------
        bool        load_public( const std::string& key, const bool as_base64 = true );

        ///-------------------------------------------------------------------------------------------------
        /// Loads the private key.
        ///
        /// @param  key         The key.
        /// @param  as_base64   (Optional) Is the key base64 encoded?
        ///
        /// @return True if it succeeds, false if it fails.
        ///-------------------------------------------------------------------------------------------------

        bool        load_private( const std::string& key, const bool as_base64 = true );

        ///-------------------------------------------------------------------------------------------------
        /// Gets the private.
        ///
        /// @return The private key as base64.
        ///-------------------------------------------------------------------------------------------------
        std::string get_private() const;

        ///-------------------------------------------------------------------------------------------------
        /// Gets the public key as base64 string.
        ///
        /// @return The public.
        ///-------------------------------------------------------------------------------------------------
        std::string get_public() const;

    public:
        /// The public key.
        std::string pub;
        /// The private key.
        std::string priv;
    };

public:
    ///-------------------------------------------------------------------------------------------------
    /// Encrypts a string with RSA.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   19.01.2017
    ///
    /// @param          plain   The plain.
    /// @param [in,out] kp      The kp.
    ///
    /// @return The RSA cipher of the plain text if succeeded, empty string otherwise.
    ///-------------------------------------------------------------------------------------------------
    static std::string encrypt( const std::string& plain, KeyPair& kp );

    ///-------------------------------------------------------------------------------------------------
    /// Encrypts a string with RSA.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   19.01.2017
    ///
    /// @param  plain                   The plain.
    /// @param  public_key              The public key.
    /// @param  public_key_as_base64    (Optional) The fourth public key as base 6.
    ///
    /// @return The RSA cipher of the plain text if succeeded, empty string otherwise.
    ///-------------------------------------------------------------------------------------------------
    static std::string encrypt( const std::string& plain,
                                const std::string& public_key,
                                const bool public_key_as_base64 = false );

    ///-------------------------------------------------------------------------------------------------
    /// Decrypts a RSA cipher.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   19.01.2017
    ///
    /// @param          cipher  The cipher.
    /// @param [in,out] kp      The kp.
    ///
    /// @return The decrypted cipher if succeeded, empty string otherwise.
    ///-------------------------------------------------------------------------------------------------
    static std::string decrypt( const std::string& cipher, KeyPair& kp );

    ///-------------------------------------------------------------------------------------------------
    /// Decrypts a RSA cipher.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   19.01.2017
    ///
    /// @param  cipher                  The cipher.
    /// @param  private_key             The private key.
    /// @param  private_key_as_base64   (Optional) The fourth private key as base 6.
    ///
    /// @return The decrypted cipher if succeeded, empty string otherwise.
    ///-------------------------------------------------------------------------------------------------
    static std::string decrypt( const std::string& cipher,
                                const std::string& private_key,
                                const bool private_key_as_base64 = false );
};
