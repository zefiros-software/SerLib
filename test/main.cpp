#include "gtest/gtest.h"
#include "types.h"
#include "message.h"
#include "helper.h"
#include "util.h"

class A
    : public ISerialisable
{
public:

    void OnSerialise( Message &message )
    {
        message.CreateRepeated( Type::WORD, ( uint32_t )mStrings.size(), 1 );
        mStrings.resize( message.Count( 1 ) );

        for ( auto it = mStrings.begin(), end = mStrings.end(); it != end; ++it )
        {
            message.StoreRepeated( *it, 1, ( uint32_t )( it - mStrings.begin() ) );
        }
    }

    void Fill()
    {
        for ( uint32_t i = 0; i < 1000; ++i )
        {
            mStrings.push_back( std::rand() );
        }
    }

private:
    std::vector< uint16_t > mStrings;
};

int main( int argc, char **argv )
{
    testing::InitGoogleTest( &argc, argv );

    A a, b;
    a.Fill();
    SimpleSerialiseDeserialiseStream( a, b );

    RUN_ALL_TESTS();

    std::getchar();

    return 0;
}