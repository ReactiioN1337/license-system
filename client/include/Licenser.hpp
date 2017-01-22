#pragma once
#include "crypto/AES.hpp"
#include "crypto/RSA.hpp"

#ifndef WEB_ENCODER
#include "Socket.hpp"
#include "WMICHwid.hpp"

enum eLicenseCode : size_t
{
    Valid                         = 0,
    Exception                     = 1,
    FailedToSendRequest           = 2,
    FailedToParseResponse         = 3,
    InvalidHwid                   = 4,
    InvalidPassword               = 5,
    InvalidUser                   = 6,
    Banned                        = 7,
    FailedToBuildHwidHash         = 8,
    Server_FailedToEncryptData    = 9,
    MySQL_FailedToUpdatePassword  = 10,
    MySQL_FailedToIncremetCounter = 11,
    MySQL_FailedToResetCounter    = 12,
    MySQL_FailedToUpdateHwid      = 13,
};

enum class eRequest : size_t
{
    Login = 0,
};

class Licenser
{
    /// The PHP file name on the server.
    static constexpr char* PHP_FILE = "test.php";

    /// The get parameter.
    static constexpr char* GET_PARAMETER = "data";

    using AESKeyPair_t = crypto::AES::KeyPair;
    using RSAKeyPair_t = crypto::RSA::KeyPair;
    using Licenser_t   = std::unique_ptr<Licenser>;

public:
    Licenser() = delete;
    explicit Licenser( const std::string& server_url );
    bool              user_login( const std::string& username, const std::string& password );
    bool              load_rsa_public_key( const std::string& key );

    static Licenser_t create_licenser( const std::string& server_url, const std::string& rsa_public_key );
    template<bool kill_process = true, typename ...Args>
    static bool       msg_box( const char* msg, const Args& ...args );

private:
    std::string       encrypt( const std::string& data, const bool rsa = false );
    std::string       decrypt( const std::string& cipher, const bool rsa = false );
    std::string       build_query( const eRequest request, const nlohmann::json& init_data = {} );
    void              encrypt_json( nlohmann::json& j );

protected:
    /// The aes key pair (key/iv).
    AESKeyPair_t m_AES;
    /// The rsa key pair (public/private).
    RSAKeyPair_t m_RSA;
    /// The windows wmic hwid wrapper.
    WMICHwid     m_HWID;
    /// The socket.
    Socket       m_Socket;    
};

template<bool kill_process, typename ...Args>
bool Licenser::msg_box( const char* msg, const Args& ...args )
{
    MessageBoxA( nullptr, tfm::format( msg, args... ).c_str(), "Error", MB_ICONERROR );
    if( kill_process ) {
        quick_exit( EXIT_FAILURE );
    }
    return false;
}

#endif
