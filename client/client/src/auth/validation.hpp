#pragma once
#include "hwid.hpp"
#include "../crypto/sealedbox.hpp"

namespace ls {

class validation
{
public:
    using subscription_type = std::tuple<std::string, std::size_t, std::int64_t>;

private:
    enum class api_code
    {
        success,
        corrupted_request,
        crypto_box_seal_failed,
        crypto_box_seal_open_failed,
        user_already_exists,
        user_not_found,
        user_is_banned,
        user_hwid_mismatch,
        user_has_no_subscription,
        product_already_exists,
        product_not_found,
        product_not_active,
        product_has_no_file_name,
        product_has_no_file,
        subscription_already_exists,
        failed_to_check_path,
        failed_to_create_path,
        failed_to_read_file,
        failed_to_write_file,
        no_api_access,
        invalid_token,
        sql_not_connected,
        sql_failed_to_execute_query,
        sql_null_rows,
        client_empty_resonse,
        client_bad_response,
        client_weird_response,
    };

    enum class request_type
    {
        login,
        get_subs,
        get_product,
    };

public:
    validation(
        const std::string_view& server_url,
        const std::string_view& server_public_key );

    bool login(
        const std::string_view& username );

    bool get_subscriptions(
        std::vector<subscription_type>& subs ) const;

    bool get_product(
        std::size_t  id,
        std::string& data ) const;

    nlohmann::json make_request(
        request_type               type,
        const nlohmann::json&      data = {} ) const;

private:
    std::string _username;
    std::size_t _user_id = 0;
    std::string _server_url;
    sealedbox::key_pair _server_key;
};

}
