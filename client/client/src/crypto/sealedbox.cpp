#include "sealedbox.hpp"
using namespace ls;
using namespace sealedbox;

key_pair::key_pair()
{
    generate();

    crypto_box_keypair( public_key.get(), private_key.get() );
}

key_pair::~key_pair()
{
    if( public_key ) {
        std::memset( public_key.get(), 0, crypto_box_PUBLICKEYBYTES );
    }
    if( private_key ) {
        std::memset( private_key.get(), 0, crypto_box_SECRETKEYBYTES );
    }
}

void key_pair::generate()
{
    if( !public_key ) {
        public_key = std::make_unique<std::uint8_t[]>( crypto_box_PUBLICKEYBYTES );
    }
    if( !private_key ) {
        private_key = std::make_unique<std::uint8_t[]>( crypto_box_SECRETKEYBYTES );
    }


}
void key_pair::load_public_key(
    const std::string_view& key ) const
{
    if( !key.empty() && public_key ) {
        auto tmp = base64::base64_to_array<std::array<std::uint8_t, crypto_box_PUBLICKEYBYTES>>( key.data() );
        std::memcpy( public_key.get(), tmp.data(), tmp.size() );
        std::memset( tmp.data(), 0, tmp.size() );
    }

}

void key_pair::load_private_key(
    const std::string_view& key ) const
{
    if( !key.empty() && private_key ) {
        auto tmp = base64::base64_to_array<std::array<std::uint8_t, crypto_box_SECRETKEYBYTES>>( key.data() );
        std::memcpy( private_key.get(), tmp.data(), tmp.size() );
        std::memset( tmp.data(), 0, tmp.size() );
    }
}

std::string key_pair::get_public_key() const
{
    return base64::encode( public_key.get(), crypto_box_PUBLICKEYBYTES );
}

std::string key_pair::get_private_key() const
{
    return base64::encode( private_key.get(), crypto_box_SECRETKEYBYTES );
}

std::string sealedbox::encrypt(
    const std::string_view& plain,
    const key_pair&         kp )
{
    if( plain.empty() || !kp.public_key ) {
        return {};
    }

    try {
        std::string cipher( plain.length() + crypto_box_SEALBYTES, 0 );
        crypto_box_seal(
            reinterpret_cast<std::uint8_t*>( cipher.data() ),
            reinterpret_cast<const std::uint8_t*>( plain.data() ),
            plain.length(),
            kp.public_key.get()
        );
        return base64::encode( cipher );
    }
    catch( ... ) {
        return {};
    }
}

std::string sealedbox::decrypt(
    const std::string_view& cipher,
    const key_pair&         kp )
{
    if( cipher.empty() || !kp.public_key || !kp.private_key ) {
        return {};
    }

    try {
        const auto cipher_decoded   = base64::decode( cipher.data() );

        std::string plain( cipher_decoded.length() - crypto_box_SEALBYTES, 0 );
        crypto_box_seal_open(
            reinterpret_cast<std::uint8_t*>( plain.data() ),
            reinterpret_cast<const std::uint8_t*>( cipher_decoded.data() ),
            cipher_decoded.size(),
            kp.public_key.get(),
            kp.private_key.get()
        );
        return plain;
    }
    catch( ... ) {
        return {};
    }
}
