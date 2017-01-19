#include <crypto/RSA.hpp>
#include <crypto/base64.hpp>
using namespace crypto;

bool RSA::KeyPair::load_private( const std::string& key, const bool as_base64 )
{
    if( !key.empty() ) {
        priv.assign( as_base64 ? base64::decode( key ) : key );
    }    
    return !priv.empty();
}

bool RSA::KeyPair::load_public( const std::string& key, const bool as_base64 )
{
    if( !key.empty() ) {
        pub.assign( as_base64 ? base64::decode( key ) : key );
    }
    return !pub.empty();
}

std::string RSA::KeyPair::get_private() const
{
    return base64::encode( priv );
}

std::string RSA::KeyPair::get_public() const
{
    return base64::decode( pub );
}

std::string RSA::encrypt( const std::string& plain, KeyPair& kp )
{
    return encrypt( plain, kp.pub );
}

std::string RSA::encrypt( const std::string& plain, const std::string& public_key, const bool public_key_as_base64 )
{
    std::string cipher;
    if( !plain.empty() && !public_key.empty() ) {
        try {
            CryptoPP::RSA::PublicKey pk;
            CryptoPP::AutoSeededRandomPool rng;
            CryptoPP::StringSource key( ( public_key_as_base64 ? base64::decode( public_key ) : public_key ), true );
            pk.Load( key );
            CryptoPP::RSAES_OAEP_SHA_Encryptor e( pk );
            CryptoPP::StringSource ss( plain,
                                       true,
                                       new CryptoPP::PK_EncryptorFilter( rng, e,
                                       new CryptoPP::StringSink( cipher ) ) );
            return base64::encode( cipher, false );
        }
        catch( CryptoPP::Exception& e ) {
            tfm::printfln( "RSA::encrypt() throwed an exception from CryptoPP: %s", e.what() );
        }
    }
    return cipher;

}

std::string RSA::decrypt( const std::string& cipher, KeyPair& kp )
{
    return decrypt( cipher, kp.priv );
}

std::string RSA::decrypt( const std::string& cipher, const std::string& private_key, const bool private_key_as_base64 )
{
    std::string plain;
    if( !cipher.empty() && !private_key.empty() ) {
        try {

            CryptoPP::RSA::PrivateKey sk;
            CryptoPP::AutoSeededRandomPool rng;
            CryptoPP::StringSource key( ( private_key_as_base64 ? base64::decode( private_key ) : private_key ), true );
            sk.Load( key );
            CryptoPP::RSAES_OAEP_SHA_Decryptor d( sk );
            CryptoPP::StringSource ss( base64::decode( cipher ),
                                       true,
                                       new CryptoPP::PK_DecryptorFilter( rng,
                                       d,
                                       new CryptoPP::StringSink( plain ) ) );
        }
        catch( CryptoPP::Exception& e ) {
            tfm::printfln( "RSA::decrypt() throwed an exception from CryptoPP: %s", e.what() );
        }
    }
    return plain;
}
