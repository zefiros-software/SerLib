#pragma once
#ifndef __NUMSERIALISEDATA_H__
#define __NUMSERIALISEDATA_H__

#include "serialiseData.h"

template< typename U, Type T >
class NumSerialiseData
    : public ISerialiseData
{
public:

    void SetValue( U val )
    {
        mValue = val;
    }

    U GetValue() const
    {
        return mValue;
    }

    virtual Type GetType() const
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

typedef NumSerialiseData< U8, Type::Char > CharSerialiseData;
typedef NumSerialiseData< U16, Type::WORD > WORDSerialiseData;
typedef NumSerialiseData< U32, Type::DWORD > DWORDSerialiseData;
typedef NumSerialiseData< U64, Type::DWORDLONG > DWORDLONGSerialiseData;

#endif