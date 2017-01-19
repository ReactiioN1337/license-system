#include <crypto/base64.hpp>
using namespace crypto;

std::string base64::url_encode( const std::string& plain )
{
    std::ostringstream oss;
    oss.fill( '0' );
    oss << std::hex;
    for( auto i = plain.begin(), n = plain.end(); i != n; ++i ) {
        const auto& c = *i;        
        if( std::isalnum( c )      ||
                          c == '-' ||
                          c == '_' ||
                          c == '.' ||
                          c == '~' ) {

            oss << c;
            continue;
        }
        oss << std::uppercase
            << '%'
            << std::setw( 2 )
            << static_cast<int32_t>( static_cast<uint8_t>( c ) )
            << std::nouppercase;
    }
    return oss.str();
}

std::string base64::encode( const std::string& plain, bool urlEncode )
{
    std::string encoded;    
    if( !plain.empty() ) {
        try {
            CryptoPP::StringSource ss( plain,
                                       true,
                                       new CryptoPP::Base64Encoder(
                                       new CryptoPP::StringSink( encoded ),
                                       false ) );
            if( urlEncode ) {
                return url_encode( encoded );
            }
        }
        catch( const CryptoPP::Exception& e ) {
            tfm::printfln( "base64::encode() throwed an exception from CryptoPP: %s", e.what() );
        }
    }    
    return encoded;
}

std::string base64::encode( const uint8_t* data, const size_t length, const bool encode_for_url )
{
    std::string encoded;
    if( data && length > 0 ) {
        try {
            CryptoPP::ArraySource ss( data, length, true,
                                      new CryptoPP::Base64Encoder(
                                      new CryptoPP::StringSink( encoded ),
                                      false ) );
            if( encode_for_url ) {
                return url_encode( encoded );
            }
        }
        catch( const CryptoPP::Exception& e ) {
            tfm::printfln( "base64::encode() throwed an exception from CryptoPP: %s", e.what() );
        }
    }
    return encoded;
}

std::string base64::decode( const std::string& encoded )
{
    std::string plain;
    if( !encoded.empty() ) {
        try {
            CryptoPP::StringSource ss( encoded, true,
                                       new CryptoPP::Base64Decoder(
                                       new CryptoPP::StringSink( plain ) ) );
        }
        catch( const CryptoPP::Exception& e ) {
            tfm::printfln( "base64::decode() throwed an exception from CryptoPP: %s", e.what() );
        }
    }    
    return plain;
}

base64::Binary_t base64::decode_binary( const std::string& encoded )
{
    const auto plain = decode( encoded );
    return Binary_t( plain.begin(), plain.end() );
}
