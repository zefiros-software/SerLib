#include "helper.h"

template<>
float GetRandom()
{
    // return with max an arbitrary number
    return ( float )rand() / ( float )( RAND_MAX / ( 1e-8 / 3.0f ) );
}

template<>
double GetRandom()
{
    // return with max an arbitrary number
    return ( float )rand() / ( float )( RAND_MAX / ( 1e-16 / 3.0f ) );
}
