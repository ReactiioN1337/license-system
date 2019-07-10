#pragma once
#include "base64.hpp"

namespace ls::sealedbox {

struct key_pair
{
    using key_type = std::unique_ptr<std::uint8_t[]>;

    key_pair();
    ~key_pair();

    void generate();
    void load_public_key(
        const std::string_view& key ) const;
    void load_private_key(
        const std::string_view& key ) const;
    no_discard
    std::string get_public_key() const;
    no_discard
    std::string get_private_key() const;

    key_type public_key = nullptr;
    key_type private_key = nullptr;
};

extern std::string encrypt(
    const std::string_view& plain,
    const key_pair&         kp );

extern std::string decrypt(
    const std::string_view& cipher,
    const key_pair&         kp );

}
