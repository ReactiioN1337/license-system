#include <WebEncoder.hpp>
#if defined(WEB_ENCODER)
#include <crypto/base64.hpp>
#include <fstream>

WebEncoder::WebEncoder() :
    m_Options( cxxopts::Options( "", "" ) )
{
    m_Options.add_options()
    ( "e,enc",       "Encrypt procedure", cxxopts::value<std::string>(), "string(aes|rsa)" )
    ( "d,dec",       "Decrypt procedure", cxxopts::value<std::string>(), "string(aes|rsa)" )
    ( "g,generate",  "Generates a new AES key/iv" )
    ( "u,urlencode", "URL encode the base64 cipher text" )
    ( "i,input",     "Input data",        cxxopts::value<std::string>(), "string" )
    ( "public",      "RSA public key",    cxxopts::value<std::string>(), "string" )
    ( "private",     "RSA private key",   cxxopts::value<std::string>(), "string" )
    ( "key",         "AES key",           cxxopts::value<std::string>(), "string" )
    ( "iv",          "AES iv",            cxxopts::value<std::string>(), "string" )
    ( "help",        "Shows all available commands" );
}

int32_t WebEncoder::handle( int32_t argc, char** argv )
{
    m_Options.parse( argc, argv );
    if( m_Options.count( "help" ) != 0 ) {
        return leave_msg<EXIT_SUCCESS>( m_Options.help( { "", "Group" } ).c_str() );
    }
    if( m_Options.count( "g" ) != 0 ) {
        return handle_generate_aes_key_pair();
    }
    if( m_Options.count( "e" ) != 0 ) {
        return handle_encryption();
    }
    if( m_Options.count( "d" ) != 0 ) {
        return handle_decryption();
    }
    
    return 1;
}

int32_t WebEncoder::handle_encryption()
{
    if( !m_Options.count( "e" ) ) {
        return EXIT_FAILURE;
    }
    if( !m_Options.count( "i" ) ) {
        return EXIT_FAILURE;
    }

    const auto input = m_Options[ "i" ].as<std::string>();
    if( input.empty() ) {
        return EXIT_FAILURE;
    }

    const auto& enc_mode = m_Options[ "e" ].as<std::string>();
    const auto rsa       = enc_mode.compare( "rsa" ) == 0;
    if( !rsa && enc_mode.compare( "aes" ) != 0 ) {
        return EXIT_FAILURE;
    }
    if( rsa ) {
        if( !m_Options.count( "public" ) ) {
            return EXIT_FAILURE;
        }
        if( !m_RSA.load_public( m_Options[ "public" ].as<std::string>() ) ) {
            return EXIT_FAILURE;
        }
    }
    else {
        if( m_Options.count( "key" ) != 0 && m_Options.count( "iv" ) != 0 ) {
            if( !m_AES.load_key( m_Options[ "key" ].as<std::string>() ) ||
                !m_AES.load_iv( m_Options[ "iv" ].as<std::string>() ) ) {
                return EXIT_FAILURE;
            }
        }
        else {
            if( handle_generate_aes_key_pair() != EXIT_SUCCESS ) {
                return EXIT_FAILURE;
            }
        }
    }

    const auto cipher = encrypt( input, rsa );
    return leave_msg<EXIT_SUCCESS>( ( m_Options.count( "u" ) != 0 ? crypto::base64::url_encode( cipher ) : cipher ).c_str() );
}

int32_t WebEncoder::handle_decryption()
{
    if( !m_Options.count( "d" ) ) {
        return EXIT_FAILURE;
    }
    if( !m_Options.count( "i" ) ) {
        return EXIT_FAILURE;
    }

    const auto input = m_Options[ "i" ].as<std::string>();
    if( input.empty() ) {
        return EXIT_FAILURE;
    }

    const auto& dec_moder = m_Options[ "d" ].as<std::string>();
    const auto rsa = dec_moder.compare( "rsa" ) == 0;
    if( !rsa && dec_moder.compare( "aes" ) != 0 ) {
        return EXIT_FAILURE;
    }
    if( rsa ) {
        if( !m_Options.count( "private" ) ) {
            return EXIT_FAILURE;
        }
        if( !m_RSA.load_private( m_Options[ "private" ].as<std::string>() ) ) {
            return EXIT_FAILURE;
        }
    }
    else {
        if( m_Options.count( "key" ) != 0 && m_Options.count( "iv" ) != 0 ) {
            if( !m_AES.load_key( m_Options[ "key" ].as<std::string>() ) ||
                !m_AES.load_iv( m_Options[ "iv" ].as<std::string>() ) ) {
                return EXIT_FAILURE;
            }
        }
    }
    return leave_msg<EXIT_SUCCESS>( decrypt( input, rsa ).c_str() );
}

int32_t WebEncoder::handle_generate_aes_key_pair()
{
    std::ofstream file( "keypair.txt", std::ios_base::out | std::ios_base::ate );
    if( !file ) {
        return EXIT_FAILURE;
    }

    file << generate_key_pair();
    file.close();

    return EXIT_SUCCESS;    
}

std::string WebEncoder::encrypt( const std::string& data, const bool rsa )
{
    return rsa
         ? crypto::RSA::encrypt( data, m_RSA )
         : crypto::AES::encrypt( data, m_AES );
}

std::string WebEncoder::decrypt( const std::string& cipher, const bool rsa )
{
    return rsa
         ? crypto::RSA::decrypt( cipher, m_RSA )
         : crypto::AES::decrypt( cipher, m_AES );
}

std::string WebEncoder::generate_key_pair()
{
    const auto tm            = std::time( nullptr );
    const auto* current_time = std::localtime( &tm );
    m_AES.generate();
    std::stringstream ss;
    ss << "///------------------------------------------------------------------\n"
       << "/// AES Key/IV\n"
       << "/// Generated: "
       << std::asctime( current_time )
       << "\n///------------------------------------------------------------------\n\n"
       << "/// 32 byte key as base64 encoded string\n"
       << m_AES.get_key()
       << "\n\n/// 16 byte iv as base64 encoded string\n"
       << m_AES.get_iv()
       << std::endl;
    return ss.str();
}

template<int32_t exit_code, typename ...Args>
int32_t WebEncoder::leave_msg( const char* msg, const Args& ...args )
{
    tfm::printfln( msg, args... );
    return exit_code;
}

#endif
