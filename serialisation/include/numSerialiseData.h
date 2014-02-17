#pragma once
#ifndef __NUMSERIALISEDATA_H__
#define __NUMSERIALISEDATA_H__

#include "serialiseData.h"

template< typename U, Type::Type T >
class NumSerialiseData
    : public ISerialiseData
{
public:

    template< typename D >
    void Store( D &val, Mode::Mode mode )
    {
        switch ( mode )
        {
        case Mode::Serialise:
            mValue = ( U )val;
            break;

        case Mode::Deserialise:
            val = ( D )mValue;
            break;
        }
    }

    virtual Type::Type GetType() const
    {
        return T;
    }

    virtual size_t Size() const
    {
        return sizeof( U );
    }

    virtual void ReadFromStream( std::istream &stream )
    {
        stream.read( ( char * )&mValue, sizeof( U ) );
    }

    virtual void WriteToStream( std::ostream &stream ) const
    {
        stream.write( ( const char * )&mValue, sizeof( U ) );
    }

protected:

    U mValue;
};

typedef NumSerialiseData< uint8_t, Type::Char > CharSerialiseData;
typedef NumSerialiseData< uint16_t, Type::WORD > WORDSerialiseData;
typedef NumSerialiseData< uint32_t, Type::DWORD > DWORDSerialiseData;
typedef NumSerialiseData< uint64_t, Type::QWORD > QWORDSerialiseData;

#endif