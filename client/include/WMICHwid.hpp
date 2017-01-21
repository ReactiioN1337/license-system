#pragma once
#include "stdafx.hpp"

#ifndef WEB_ENCODER

///-------------------------------------------------------------------------------------------------
/// Windows WMIC wrapper class by ReactiioN. 
///
/// @author ReactiioN (https://github.com/ReactiioN1337)
///         scumbucket(rifk)
/// @date   21.01.2017
///-------------------------------------------------------------------------------------------------
class WMICHwid
{
public:
    WMICHwid() = default;
    ///-------------------------------------------------------------------------------------------------
    /// Queries this object.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   21.01.2017
    ///
    /// @return True if it succeeds, false if it fails.
    ///-------------------------------------------------------------------------------------------------
    bool                      query();

    ///-------------------------------------------------------------------------------------------------
    /// Gets the CPU.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   21.01.2017
    ///
    /// @return A reference to a const std::string.
    ///-------------------------------------------------------------------------------------------------
    inline const std::string& cpu() const;

    ///-------------------------------------------------------------------------------------------------
    /// Computer name.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   21.01.2017
    ///
    /// @return A reference to a const std::string.
    ///-------------------------------------------------------------------------------------------------
    inline const std::string& computer_name() const;

    ///-------------------------------------------------------------------------------------------------
    /// Physical HDD serial.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   21.01.2017
    ///
    /// @return A reference to a const std::string.
    ///-------------------------------------------------------------------------------------------------
    inline const std::string& physical_hdd_serial() const;

private:
    ///-------------------------------------------------------------------------------------------------
    /// Queries a wmic data.
    ///
    /// @author scumbucket(rifk) (https://high-minded.net/threads/hwids-time-savvy-method.55401/)
    /// @date   21.01.2017
    ///
    /// @param          input   The input.
    /// @param [in,out] out     The out.
    ///
    /// @return True if it succeeds, false if it fails.
    ///-------------------------------------------------------------------------------------------------

    static bool               query_wmic( const std::string& input,
                                          std::string&       out );

protected:
    std::string m_CPU;
    std::string m_ComputerName;
    std::string m_Physical;
};

inline const std::string& WMICHwid::cpu() const
{
    return m_CPU;
}

inline const std::string& WMICHwid::computer_name() const
{
    return m_ComputerName;
}

inline const std::string& WMICHwid::physical_hdd_serial() const
{
    return m_Physical;
}

#endif
