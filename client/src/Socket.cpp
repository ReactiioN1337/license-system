#include <Socket.hpp>

#ifndef WEB_ENCODER
Socket::Socket( const std::string& server_url ) :
    m_ServerUrl( server_url )
{
}

bool Socket::http_get( const std::string& file,
                       const std::string& data,
                       std::string&       response )
{
    if( file.empty() || data.empty() ) {
        return false;
    }

    auto query = file;
    if( query.back() != '?' || data.front() != '?' ) {
        query.append( "?" );
    }
    query.append( data );

    WSADATA win_socket_data;
    if( WSAStartup( MAKEWORD( 2, 2 ), &win_socket_data ) != 0 ) {
        return false;
    }

    addrinfo hints, *addr_info;
    RtlSecureZeroMemory( &hints, sizeof( hints ) );

    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    if( getaddrinfo( m_ServerUrl.c_str(), "80", &hints, &addr_info ) != 0 ) {
        return free_socket_data();
    }

    m_Socket = socket( addr_info->ai_family,
                       addr_info->ai_socktype,
                       addr_info->ai_protocol );
    if( m_Socket == SOCKET_INVALID ) {
        return free_socket_data( true, false, reinterpret_cast<void**>( &addr_info ) );
    }
    if( connect( m_Socket,
                 addr_info->ai_addr,
                 static_cast<int32_t>( addr_info->ai_addrlen ) ) != 0 ) {
        return free_socket_data( true, true, reinterpret_cast<void**>( &addr_info ) );
    }
    freeaddrinfo( addr_info );

    std::string http_header = "GET /";
    http_header.append( query ).append( " HTTP/1.0\r\nHost: " ).append( m_ServerUrl );

    /// Append \r\n\r\n to our response to identfy the end of the http header inside the response
    http_header.append( "\r\n\r\n" );

    std::array<char, 512> buffer;
    if( send( m_Socket,
              http_header.c_str(),
              static_cast<int32_t>( http_header.size() ), 0 ) == SOCKET_ERROR ) {
        return free_socket_data( true, true );
    }
    
    size_t bytes_received;
    do {
        bytes_received = static_cast<size_t>( recv( m_Socket,
                                                    buffer.data(),
                                                    buffer.size(), 0 ) );
        response.append( buffer.data(), bytes_received );

    } while( bytes_received > 0 );    
    for( size_t i = 0; i < response.length() - 4; ++i ) {
        if( response.at( i ) == '\r'     &&
            response.at( i + 1 ) == '\n' &&
            response.at( i + 2 ) == '\r' &&
            response.at( i + 3 ) == '\n' ) {
            response = response.substr( i + 4, response.length() );
            break;
        }
    }

    return free_socket_data<true>( true, true );
}

bool Socket::is_valid() const
{
    return m_Socket != SOCKET_INVALID;
}

template<bool RET>
bool Socket::free_socket_data( const bool clear,
                               const bool disconnect,
                               void**     addr_info_data )
{
    if( addr_info_data && *addr_info_data ) {
        freeaddrinfo( reinterpret_cast<addrinfo*>( *addr_info_data ) );
    }
    if( disconnect && is_valid()  ) {
        closesocket( static_cast<SOCKET>( m_Socket ) );
        m_Socket = SOCKET_INVALID;
    }
    if( clear ) {
        WSACleanup();
    }
    return RET;
}

#endif
