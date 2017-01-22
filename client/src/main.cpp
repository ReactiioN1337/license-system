#if defined(WEB_ENCODER)
#include <WebEncoder.hpp>
#else
#include <stdafx.hpp>
#endif

int32_t main( int32_t argc, char** argv )
{
#if defined(WEB_ENCODER)
    WebEncoder enc;
    return enc.handle( argc, argv );
#else
    UNREFERENCED_PARAMETER( argc );
    UNREFERENCED_PARAMETER( argv );
    return EXIT_SUCCESS;
#endif
}
