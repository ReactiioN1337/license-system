#pragma once
#include "stdafx.hpp"
#if defined(WEB_ENCODER)
#include "crypto/RSA.hpp"
#include "crypto/AES.hpp"

class WebEncoder
{
public:
    ///-------------------------------------------------------------------------------------------------
    /// Default constructor.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   21.01.2017
    ///-------------------------------------------------------------------------------------------------
    WebEncoder();

    ///-------------------------------------------------------------------------------------------------
    /// Handles the command line.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   21.01.2017
    ///
    /// @param          argc    The argc.
    /// @param [in,out] argv    If non-null, the argv.
    ///
    /// @return An int32_t.
    ///-------------------------------------------------------------------------------------------------
    int32_t        handle( int32_t argc, char** argv );

private:
    ///-------------------------------------------------------------------------------------------------
    /// Handles the encryption.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   21.01.2017
    ///
    /// @return An int32_t.
    ///-------------------------------------------------------------------------------------------------
    int32_t        handle_encryption();

    ///-------------------------------------------------------------------------------------------------
    /// Handles the decryption.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   21.01.2017
    ///
    /// @return An int32_t.
    ///-------------------------------------------------------------------------------------------------
    int32_t        handle_decryption();

    ///-------------------------------------------------------------------------------------------------
    /// Handles the generate aes key pair.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   21.01.2017
    ///
    /// @return An int32_t.
    ///-------------------------------------------------------------------------------------------------
    int32_t        handle_generate_aes_key_pair();

    ///-------------------------------------------------------------------------------------------------
    /// Encrypts a plain string.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   21.01.2017
    ///
    /// @param  data    The data.
    /// @param  rsa     Cipher as rsa.
    ///
    /// @return A std::string.
    ///-------------------------------------------------------------------------------------------------
    std::string    encrypt( const std::string& data, const bool rsa );

    ///-------------------------------------------------------------------------------------------------
    /// Decrypts a cipher.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   21.01.2017
    ///
    /// @param  cipher  The cipher.
    /// @param  rsa     Cipher is rsa.
    ///
    /// @return A std::string.
    ///-------------------------------------------------------------------------------------------------
    std::string    decrypt( const std::string& cipher, const bool rsa );

    ///-------------------------------------------------------------------------------------------------
    /// Generates a new aes key pair (key/iv).
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   21.01.2017
    ///
    /// @return The key pair text message file.
    ///-------------------------------------------------------------------------------------------------
    std::string    generate_key_pair();

    ///-------------------------------------------------------------------------------------------------
    /// Leave message.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   21.01.2017
    ///
    /// @tparam exit_code   Type of the exit code.
    /// @tparam ...Args     Type of the ... arguments.
    /// @param  msg     The message.
    /// @param  ...args The .. .args.
    ///
    /// @return An int32_t.
    ///-------------------------------------------------------------------------------------------------
    template<int32_t exit_code = EXIT_FAILURE, typename ...Args>
    static int32_t leave_msg( const char* msg, const Args& ...args );

protected:
    cxxopts::Options     m_Options;
    crypto::AES::KeyPair m_AES;
    crypto::RSA::KeyPair m_RSA;
};

#endif
