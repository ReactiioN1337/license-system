#include "hwid.hpp"
#include "../crypto/xorstr.hpp"
using namespace ls;

std::string hwid::get_super_secret_info()
{
    char root[ MAX_PATH + 1 ]{};
    GetEnvironmentVariableA( xors( "HOMEDRIVE" ), root, MAX_PATH );
    std::strcat( root, xors( "\\" ) );

    DWORD volume_serial_number = 0;
    GetVolumeInformationA( root, nullptr, 0, &volume_serial_number, nullptr, nullptr, nullptr, 0 );

    return std::to_string( volume_serial_number );
}
