#include "base64.hpp"
using namespace ls;

std::string base64::encode(
    const std::string_view& data )
{
    return encode( reinterpret_cast<const std::uint8_t*>( data.data() ), data.size() );
}

std::string base64::encode(
    const binary_type& data )
{
    return encode( data.data(), data.size() );
}

std::string base64::encode(
    const std::uint8_t* data,
    const std::size_t   len )
{
    static constexpr char encoding_table[] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
        'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
        'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
        'w', 'x', 'y', 'z', '0', '1', '2', '3',
        '4', '5', '6', '7', '8', '9', '+', '/'
    };

    const auto out_len = 4 * ( ( len + 2 ) / 3 );
    std::string ret( out_len, '\0' );
    std::size_t i;
    auto p = const_cast<char*>( ret.c_str() );

    for( i = 0; i < len - 2; i += 3 ) {
        *p++ = encoding_table[ ( data[ i ] >> 2 ) & 0x3F ];
        *p++ = encoding_table[ ( ( data[ i ] & 0x3 ) << 4 ) | ( static_cast<std::int32_t>( data[ i + 1 ] & 0xF0 ) >> 4 ) ];
        *p++ = encoding_table[ ( ( data[ i + 1 ] & 0xF ) << 2 ) | ( static_cast<std::int32_t>( data[ i + 2 ] & 0xC0 ) >> 6 ) ];
        *p++ = encoding_table[ data[ i + 2 ] & 0x3F ];
    }
    if( i < len ) {
        *p++ = encoding_table[ ( data[ i ] >> 2 ) & 0x3F ];
        if( i == ( len - 1 ) ) {
            *p++ = encoding_table[ ( ( data[ i ] & 0x3 ) << 4 ) ];
            *p++ = '=';
        } else {
            *p++ = encoding_table[ ( ( data[ i ] & 0x3 ) << 4 ) | ( static_cast<std::int32_t>( data[ i + 1 ] & 0xF0 ) >> 4 ) ];
            *p++ = encoding_table[ ( ( data[ i + 1 ] & 0xF ) << 2 ) ];
        }
        *p++ = '=';
    }

    return ret;
}

std::string base64::decode(
    const std::string_view& input )
{
    auto bin = decode_binary( input );
    if( bin.empty() ) {
        return "";
    }

    std::string out;
    out.assign( reinterpret_cast<const char*>( bin.data() ), bin.size() );
    return out;
}

std::string base64::url_encode(
    const std::string_view& in )
{
    static constexpr char lookup[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

    std::stringstream e;
    for( const auto& c : in ) {
        if( ( 48 <= c && c <= 57 )  || // 0-9
            ( 65 <= c && c <= 90 )  || // abc...xyz
            ( 97 <= c && c <= 122 ) || // ABC...XYZ
            ( c == '-' || c == '_' || c == '.' || c == '~' )
            ) {
            e << c;
        }
        else {
            e << '%';
            e << lookup[ ( c & 0xF0 ) >> 4 ];
            e << lookup[ ( c & 0x0F ) ];
        }
    }
    return e.str();
}

base64::binary_type base64::decode_binary(
    const std::string_view& input )
{
    static constexpr std::uint8_t encoding_table[] = {
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
        64, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
        64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
    };

    const auto in_len = input.size();
    if( in_len % 4 != 0 ) {
        return {};
    }

    auto out_len = in_len / 4 * 3;
    if( input[ in_len - 1 ] == '=' ) {
        out_len--;
    }
    if( input[ in_len - 2 ] == '=' ) {
        out_len--;
    }

    binary_type out( out_len, 0 );
    for( std::size_t i = 0, j = 0; i < in_len; ) {
        const auto a = input[ i ] == '=' ? 0 & i++ : encoding_table[ static_cast<std::int32_t>( input[ i++ ] ) ];
        const auto b = input[ i ] == '=' ? 0 & i++ : encoding_table[ static_cast<std::int32_t>( input[ i++ ] ) ];
        const auto c = input[ i ] == '=' ? 0 & i++ : encoding_table[ static_cast<std::int32_t>( input[ i++ ] ) ];
        const auto d = input[ i ] == '=' ? 0 & i++ : encoding_table[ static_cast<std::int32_t>( input[ i++ ] ) ];

        const auto triple = ( a << 3 * 6 ) + ( b << 2 * 6 ) + ( c << 1 * 6 ) + ( d << 0 * 6 );

        if( j < out_len ) {
            out[ j++ ] = ( triple >> 2 * 8 ) & 0xFF;
        }
        if( j < out_len ) {
            out[ j++ ] = ( triple >> 1 * 8 ) & 0xFF;
        }
        if( j < out_len ) {
            out[ j++ ] = ( triple >> 0 * 8 ) & 0xFF;
        }
    }

    return out;
}
