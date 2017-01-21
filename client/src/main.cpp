#include <stdafx.hpp>
#include "WMICHwid.hpp"
#include "Socket.hpp"
#include "crypto/AES.hpp"
#include "crypto/RSA.hpp"
#include "crypto/base64.hpp"
#if defined(WEB_ENCODER)
#include <WebEncoder.hpp>
#endif

#ifndef WEB_ENCODER
enum eLicenseCode : size_t
{
    Valid = 0,
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
    bool user_login( const std::string& username, const std::string& password );

    bool load_rsa_public_key( const std::string& key );
    static Licenser_t create_licenser( const std::string& server_url, const std::string& rsa_public_key );

private:
    std::string encrypt( const std::string& data, const bool rsa = false );
    std::string decrypt( const std::string& cipher, const bool rsa = false );
    std::string build_query( const eRequest request, const nlohmann::json& init_data = {} );
    void encrypt_json( nlohmann::json& j );

protected:
    /// The aes key pair (key/iv).
    AESKeyPair_t m_AES;

    /// The rsa key pair (public/private).
    RSAKeyPair_t m_RSA;

    /// The windows wmic hwid wrapper.
    WMICHwid m_HWID;

    /// The socket.
    Socket m_Socket;
    
};

Licenser::Licenser( const std::string& server_url )
{
    m_Socket.set_server_url( server_url );
}

bool Licenser::user_login( const std::string& username, const std::string& password )
{
    if( username.empty() || password.empty() ) {
        return false;
    }
    if( !m_HWID.query() ) {
        return false;
    }

    std::string response;
    if( !m_Socket.http_get( PHP_FILE, build_query( eRequest::Login, [&]{
        nlohmann::json j, hwid;
        
        hwid[ "cpu" ]                 = m_HWID.cpu();
        hwid[ "computer_name" ]       = m_HWID.computer_name();
        hwid[ "physical_hdd_serial" ] = m_HWID.physical_hdd_serial();

        j[ "username" ] = username;
        j[ "password" ] = password;
        j[ "hwid" ]     = hwid;

        return j;
    }() ), response ) ) {
        return false;
    }

    std::cout << "Response: " << response << std::endl;

    return true;
}

bool Licenser::load_rsa_public_key( const std::string& key )
{
    return m_RSA.load_public( key );
}

Licenser::Licenser_t Licenser::create_licenser( const std::string& server_url, const std::string& rsa_public_key )
{
    if( server_url.empty() || rsa_public_key.empty() ) {
        return nullptr;
    }
    auto licenser = std::make_unique<Licenser>( server_url );
    return licenser->load_rsa_public_key( rsa_public_key )
         ? std::move( licenser )
         : nullptr;
}

std::string Licenser::build_query( const eRequest request, const nlohmann::json& init_data )
{
    m_AES.generate();

    auto j = init_data;
    j[ "request" ] = std::to_string( static_cast<size_t>( request ) );
    encrypt_json( j );

    std::cout << j.dump( 2 ) << std::endl;

    j[ "key" ] = m_AES.get_key();
    j[ "iv" ]  = m_AES.get_iv();

    std::stringstream ss;
    ss << GET_PARAMETER
       << "="
       << crypto::base64::url_encode( encrypt( j.dump( 2 ), true ) );

    return ss.str();
}

std::string Licenser::encrypt( const std::string& data, const bool rsa )
{
    return rsa
         ? crypto::RSA::encrypt( data, m_RSA )
         : crypto::AES::encrypt( data, m_AES );
}

std::string Licenser::decrypt( const std::string& cipher, const bool rsa )
{
    return rsa
         ? crypto::RSA::decrypt( cipher, m_RSA )
         : crypto::AES::decrypt( cipher, m_AES );
}

void Licenser::encrypt_json( nlohmann::json& j )
{
    for( auto& j_data : j ) {
        if( j_data.is_string() ) {
            j_data = encrypt( j_data.get<std::string>() );
        }
        else if( j_data.is_object() ) {
            encrypt_json( j_data );
        }
    }
}

#endif

int32_t main( int32_t argc, char** argv )
{
#if defined(WEB_ENCODER)
    WebEncoder enc;
    return enc.handle( argc, argv );
#else
    
    auto licenser = Licenser::create_licenser( "test.reactiion.pw", "MIIEFjANBgkqhkiG9w0BAQEFAAOCBAMAMIID/gKCA/UAotWjOV+9OHN9d2ot4P9m85COuULA3YoZ1EHEZFvODdZxeS0ZzH+EiaWc7nTM7HR6t6L9R7P2Dh1mnXgrephwGZ9zmHkqbgKIJ3sv1qhNr0z2iMTQadH8FZ23OXRfFYczBLODF6KV6xYV9mPuADVcTzfh63a5Vuma41d0pD1TfB8o3g5o62p9ksYUUmQFNiiIrSdrSL8izIrILDBGRqJ5Qjo2+PJirMUdIjpKz4485Mcv9VE72yrizvrFb0WEbY4+pt3FRhhcI/giChLcAPbpMVXgB670qN7c86PVu6VI0ILUIqcZG2gDD82vaaDImpf/BWFYYt1mLkGhL59dhLMLI3QR7dCqtYtiLIm0Ci0KqkWsrFjachU61RoMzThLXVC0wv7v+ZPoKULMLUN4Sv/4AFoDwy1ozcNd0qnWhN/7aED0JOVlxZoDLQOUOh+g/PrKP0dHSbPNptA9hJJy263rdBlD7McwPFG1AjmoWVGb56u1VNE5VIB6ExHDtwwuX1Tq5ROPuS7yzB1nYQzL3fhZIjqJ6e0Mqhm2Hl4T7CDUhwuOg84LHMvP8LmK4gT4fseI1XfjQQu7RRypfJ85QiaruLxLkAlq97ZfkIrmYVKGJJ41NrwnMsFVTPIBz3F6B6KYVQ+N/HLooFj8TX6TfFwA1fPIk/kHNVDEtcWh+rIC+cFvF3fDE97pgyb3w0mLnrBrR0AqbNWyTzQ+DmfCiwhtM+XSHnmWtNBEdkI4GKFBGEpqNSiCVRyyuNu3UqvxQD2XlxOvbhwuQ0/FkCXsaez4xLBrvHhA7ClOxyYjKMsehpw/TACiXlj06opGX1yoxHpS7DoWSwAJeXtxU5PsoNEzTdX1t7WLGbXVhpQo+k6XTQ8FuIaViIBG0WjT4VJ15NHzzf1jegHo7gbi0pUjTKpInzsved6Pl8UckfGZoyvRQgTqVSLIAP1oYo160/ICO3BS33zW+Htf7Zbjljwj49uyvtnPf2moBv1YX4FhH0Bt/XjvHuf1b6OPJtDNR35NucWfu/VbeCNMv7JHgVU7UiUnsC5b0nx9CLxamPxxf48E6EWPcccfKL0oeveHK522QpG9/sKyOkUQBOSmvAHy/M4YOCqmiZIqBy4DqZ6iRzZzgBcN+eLdCOzToxFQLutMytyxqz+vm2+zyzgmwRi9Pzd9Mf+M9R3nC6GvY4s+6WExv23u6hgD5u65h3bFAsiaHGmgNrqPoGSyZgud62ThGn4rtmcBUqGFto3sgjmHRTbVEyk/SICgN5GHNqZEOSB0t44yVkv6CyZVyKGnUPH6FaHbQhTrwrNXHN0NwT0aizUOUJ3QpMmByLAnfExSPpjM0u9N/TzMyQIDAQAB" );
    licenser->user_login( "reactiion", "berserker" );

    std::cin.get();
    return EXIT_SUCCESS;
#endif
}
