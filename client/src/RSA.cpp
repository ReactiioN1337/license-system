#include <crypto/RSA.hpp>
#include <crypto/base64.hpp>
using namespace crypto;

bool RSA::KeyPair::load_private( const std::string& key )
{
    if( key.empty() ) {
        return false;
    }

    const auto decoded = base64::decode_binary( key );
    priv = CryptoPP::SecByteBlock( decoded.data(), decoded.size() );
    return true;
}

bool RSA::KeyPair::load_public( const std::string& key )
{
    if( key.empty() ) {
        return false;
    }

    const auto decoded = base64::decode_binary( key );
    pub = CryptoPP::SecByteBlock( decoded.data(), decoded.size() );
    return true;
}

std::string RSA::KeyPair::get_private() const
{
    return base64::encode( priv.data(), priv.size() );
}

std::string RSA::KeyPair::get_public() const
{
    return base64::encode( pub.data(), pub.size() );
}

std::string RSA::encrypt( const std::string& plain, KeyPair& kp )
{
    return encrypt( plain, kp.pub.data(), kp.pub.size() );
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

std::string RSA::encrypt( const std::string& plain, const uint8_t* public_key, const size_t public_key_size )
{
    std::string cipher;
    if( !plain.empty() && public_key && public_key_size > 0 ) {
        try {
            CryptoPP::RSA::PublicKey pk;
            CryptoPP::AutoSeededRandomPool rng;
            CryptoPP::ArraySource key( public_key, public_key_size, true );
            pk.Load( key );
            CryptoPP::RSAES_OAEP_SHA_Encryptor e( pk );
            CryptoPP::StringSource ss1( plain,
                                        true,
                                        new CryptoPP::PK_EncryptorFilter( rng,
                                        e,
                                        new CryptoPP::StringSink( cipher ) ) );
            return base64::encode( cipher );
        }
        catch( CryptoPP::Exception& e ) {
            tfm::printfln( "RSA::encrypt() throwed an exception from CryptoPP: %s", e.what() );
        }
    }
    return cipher;
}

std::string RSA::decrypt( const std::string& cipher, KeyPair& kp )
{
    return decrypt( cipher, kp.priv.data(), kp.priv.size() );
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

std::string RSA::decrypt( const std::string& cipher, const uint8_t* private_key, const size_t private_key_size )
{
    std::string plain;
    if( !cipher.empty() && private_key && private_key_size > 0 ) {
        try {
            CryptoPP::RSA::PrivateKey sk;
            CryptoPP::AutoSeededRandomPool rng;
            CryptoPP::ArraySource key( private_key, private_key_size, true );
            sk.Load( key );
            CryptoPP::RSAES_OAEP_SHA_Decryptor d( sk );
            CryptoPP::StringSource ss2( base64::decode( cipher ),
                                        true,
                                        new CryptoPP::PK_DecryptorFilter( rng,
                                        d,
                                        new CryptoPP::StringSink( plain ) ) );
        }
        catch( const CryptoPP::Exception& e ) {
            tfm::printfln( "RSA::decrypt() throwed an exception from CryptoPP: %s", e.what() );
        }
    }
    return plain;
}
