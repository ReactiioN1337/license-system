#include <stdafx.hpp>
#if defined(WEB_ENCODER)
#include <WebEncoder.hpp>
#endif

int32_t main( int32_t argc, char** argv )
{
#if defined(WEB_ENCODER)
    WebEncoder enc;
    return enc.handle( argc, argv );
#else
    return EXIT_SUCCESS;
#endif
}
