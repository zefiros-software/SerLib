#include <sstream>
#include <iostream>
#include "message.h"
#include <vector>
#include "numSerialiseData.h"

class A
    : public ISerialisable
{
public:

    A()
    {
        Set();
    }

    void Print() const
    {
        std::cout << a << std::endl
                  << b << std::endl
                  << c << std::endl
                  << d << std::endl
                  << e << std::endl
                  << f << std::endl
                  << std::endl;
    }

    void Set()
    {
        a =  234;
        b = -123;
        c =  32; //UINT64_MAX;
        d = -20;
        e = 123;
        f = "aapje";
    }

    void Reset()
    {
        a = 0;
        b = 0;
        c = 0;
        d = 0;
        e = 0;
        f = "";
    }

    virtual void OnSerialise( Message &message )
    {
        message.Store( &a, 1 );
        message.Store( &b, 2, 1 );
        message.Store( &c, 3, 1 );
        message.Store( &d, 4 );
        message.Store( &e, 5, 1 );
        message.Store( &f, 6 );
    }

private:

    U32 a;
    S32 b;
    U64 c;
    S16 d;
    U8 e;
    std::string f;
};

class B
    : public ISerialisable
{
public:

    virtual void OnSerialise( Message &message )
    {
        message.StoreObject( &mA, 1 );
        message.StoreObject( &mAA, 2 );
    }

    void Reset()
    {
        mA.Reset();
        mAA.Reset();
    }

    void Print()
    {
        mA.Print();
        mAA.Print();
        std::cout << "--------------------" << std::endl;
    }

private:

    A mA, mAA;
};

void main( int, const char *[] )
{
    B a;
    std::stringstream ss;

    Message message( Mode::Serialise );
    a.OnSerialise( message );
    message.WriteToStream( ss );

    a.Print();
    a.Reset();
    a.Print();

    Message message2( Mode::Deserialise );
    message2.ReadFromStream( ss );
    a.OnSerialise( message2 );

    a.Print();


    system( "Pause" );
}