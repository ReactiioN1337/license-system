#include <crypto/AES.hpp>
#include <crypto/base64.hpp>
using namespace crypto;

AES::KeyPair::KeyPair()
{
    generate();
}

AES::KeyPair::KeyPair( const std::string& key,
                       const std::string& iv )
{
    load_key( key );
    load_iv( iv );
}

bool AES::KeyPair::load_iv( const std::string& data )
{
    const auto encoded = base64::decode_binary( data );
    if( encoded.empty() ) {
        return false;
    }

    iv = CryptoPP::SecByteBlock( encoded.data(), encoded.size() );
    return true;
}

bool AES::KeyPair::load_key( const std::string& data )
{
    const auto encoded = base64::decode_binary( data );
    if( encoded.empty() ) {
        return false;
    }

    key = CryptoPP::SecByteBlock( encoded.data(), encoded.size() );
    return true;
}

std::string AES::KeyPair::get_key() const
{
    return base64::encode( key.data(), key.size(), false );
}

std::string AES::KeyPair::get_iv() const
{
    return base64::encode( iv.data(), iv.size(), false );
}

void AES::KeyPair::generate()
{
    CryptoPP::AutoSeededRandomPool rng;
    key = CryptoPP::SecByteBlock( CryptoPP::AES::MAX_KEYLENGTH );
    iv  = CryptoPP::SecByteBlock( CryptoPP::AES::BLOCKSIZE );

    rng.GenerateBlock( key, key.size() );
    rng.GenerateBlock( iv, iv.size() );
}

std::string AES::encrypt( const std::string& plain,
                          const std::string& key,
                          const std::string& iv )
{
    if( plain.empty() || key.empty() || iv.empty() ) {
        return "";
    }
    
    KeyPair kp( key, iv );
    return encrypt( plain, kp );
}

std::string AES::encrypt( const std::string& plain,
                          const uint8_t*     key,
                          const size_t       key_size,
                          const uint8_t*     iv,
                          const size_t       iv_size )
{
    std::string cipher;
    if( !plain.empty() && key && key_size > 0 && iv && iv_size > 0 ) {
        try {
            CryptoPP::GCM< CryptoPP::AES >::Encryption enc;
            enc.SetKeyWithIV( key, key_size, iv, iv_size );
            CryptoPP::AuthenticatedEncryptionFilter aef( enc,
                                                         new CryptoPP::StringSink( cipher ) );
            aef.Put( reinterpret_cast<const uint8_t*>( plain.data() ), plain.size() );
            aef.MessageEnd();
            return base64::encode( cipher );
        }
        catch( const CryptoPP::Exception& e ) {
            tfm::printfln( "AES::encrypt() throwed an exception from CryptoPP: %s", e.what() );
        }
    }
    return cipher;
}

std::string AES::encrypt( const std::string& plain,
                          KeyPair&           kp )
{
    return encrypt( plain,
                    kp.key.data(),
                    kp.key.size(),
                    kp.iv.data(),
                    kp.iv.size() );
}

std::string AES::decrypt( const std::string& cipher,
                          const std::string& key,
                          const std::string& iv )
{
    if( cipher.empty() || key.empty() || iv.empty() ) {
        return "";
    }

    KeyPair kp( key, iv );
    return decrypt( cipher, kp );
}

std::string AES::decrypt( const std::string& cipher,
                          const uint8_t*     key,
                          const size_t       key_size,
                          const uint8_t*     iv,
                          const size_t       iv_size )
{
    std::string plain;
    if( !cipher.empty() && key && key_size > 0 && iv && iv_size > 0 ) {
        try {
            const auto decoded_cipher = base64::decode_binary( cipher );
            CryptoPP::GCM< CryptoPP::AES >::Decryption dec;
            dec.SetKeyWithIV( key, key_size, iv, iv_size );
            CryptoPP::AuthenticatedDecryptionFilter adf( dec,
                                                         new CryptoPP::StringSink(
                                                         plain ) );
            adf.Put( decoded_cipher.data(), decoded_cipher.size() );
            adf.MessageEnd();            
        }
        catch( const CryptoPP::Exception& e ) {
            tfm::printfln( "AES::decrypt() throwed an exception from CryptoPP: %s", e.what() );
        }
    }return plain;
}

std::string AES::decrypt( const std::string& cipher,
                          KeyPair&          kp )
{
    return decrypt( cipher,
                    kp.key.data(),
                    kp.key.size(),
                    kp.iv.data(),
                    kp.iv.size() );
}
