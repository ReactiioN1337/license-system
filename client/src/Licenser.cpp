#include <Licenser.hpp>
#ifndef WEB_ENCODER
#include <crypto/base64.hpp>

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
        return msg_box( "failed to get the wmic hwid" );
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
        return msg_box( "failed to sent GET request" );
    }
    if( response.empty() || !response.compare( "forbidden" ) ) {
        return msg_box( "failed to parse response" );
    }
    try {
        auto j = nlohmann::json::parse( decrypt( response ) );
        const auto code = j.at( "code" ).get<size_t>();
        if( code != eLicenseCode::Valid ) {
            switch( code ) {
            case eLicenseCode::InvalidUser:
            case eLicenseCode::InvalidPassword:
                return msg_box( "invalid username or password" );

            case eLicenseCode::InvalidHwid:
                return msg_box( "invalid hardware id" );

            case eLicenseCode::Banned:
                return msg_box( "rekt m8, you're banned" );

            default:
                /// fuck %d / %lld(for x64) -> strings be St e
                return msg_box( "operation failed, error: %s", std::to_string( code ) );
            }
        }
    }
    catch( const std::exception& e ) {
        return msg_box( "operation failed, error: %s\nException: %s",
                        std::to_string( eLicenseCode::Exception ),
                        e.what()
        );
    }
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

    j[ "key" ] = m_AES.get_key();
    j[ "iv" ]  = m_AES.get_iv();

    std::stringstream ss;
    ss << GET_PARAMETER
       << "="
       << crypto::base64::url_encode( encrypt( j.dump(), true ) );

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
