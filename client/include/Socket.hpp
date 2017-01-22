#pragma once
#include "stdafx.hpp"

#ifndef WEB_ENCODER

class Socket
{
public:
    /// Same as under MSVC: INVALID_SOCKET
    /// (equal to SOCKET_ERROR, only for unsigned types)
    static constexpr uintptr_t SOCKET_INVALID = static_cast<uintptr_t>( ~0 );

private:
    /// unix may needs a different type, I haven't checked it yet.
    using Socket_t = uintptr_t;

public:
    ///-------------------------------------------------------------------------------------------------
    /// Default constructor.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   19.01.2017
    ///-------------------------------------------------------------------------------------------------
    Socket() = default;

    ///-------------------------------------------------------------------------------------------------
    /// Constructor.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   19.01.2017
    ///
    /// @param  server_url  URL of the server.
    ///-------------------------------------------------------------------------------------------------
    explicit Socket( const std::string& server_url );

    ///-------------------------------------------------------------------------------------------------
    /// Sends a HTTP GET request.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   19.01.2017
    ///
    /// @param          file        The file.
    /// @param          data        The data.
    /// @param [in,out] response    The response.
    ///
    /// @return True if it succeeds, false if it fails.
    ///-------------------------------------------------------------------------------------------------
    bool http_get( const std::string& file,
                   const std::string& data,
                   std::string&       response );

    ///-------------------------------------------------------------------------------------------------
    /// Query if this object is valid.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   19.01.2017
    ///
    /// @return True if valid, false if not.
    ///-------------------------------------------------------------------------------------------------
    bool is_valid() const;

    ///-------------------------------------------------------------------------------------------------
    /// Gets server URL.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   19.01.2017
    ///
    /// @return The server URL.
    ///-------------------------------------------------------------------------------------------------
    inline const std::string& get_server_url() const;

    ///-------------------------------------------------------------------------------------------------
    /// Sets server URL.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   19.01.2017
    ///
    /// @param  server_url  URL of the server.
    ///-------------------------------------------------------------------------------------------------
    inline void set_server_url( const std::string& server_url );

private:
    ///-------------------------------------------------------------------------------------------------
    /// Free socket data.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   19.01.2017
    ///
    /// @tparam RET Type of the ret.
    /// @param          clear           (Optional) Clean socket data.
    /// @param          disconnect      (Optional) Disconnect from socket.
    /// @param [in,out] addr_info_data  (Optional) If non-null, information describing the address
    ///                                 information.
    ///
    /// @return True if it succeeds, false if it fails.
    ///-------------------------------------------------------------------------------------------------
    template<bool RET = false>
    bool free_socket_data( const bool clear          = true,
                           const bool disconnect     = false,
                           void**     addr_info_data = nullptr );

protected:
    uintptr_t   m_Socket = SOCKET_INVALID;
    std::string m_ServerUrl;
};

inline const std::string& Socket::get_server_url() const
{
    return m_ServerUrl;
}

inline void Socket::set_server_url( const std::string& server_url )
{
    m_ServerUrl = server_url;
}

#endif
