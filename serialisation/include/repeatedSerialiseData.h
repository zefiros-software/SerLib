#pragma once
#ifndef __REPEATEDSERIALISEDATA_H__
#define __REPEATEDSERIALISEDATA_H__

#include "abstractRepeatedData.h"
#include "numSerialiseData.h"
#include "varIntSerialiseData.h"
#include "stringSerialiseData.h"
#include "util.h"
#include <assert.h>
#include "types.h"

template< typename DataType, Type::Type SubType >
class RepeatedData
    : public AbstractRepeatedData
{
public:

    virtual ISerialiseData *GetSerialisable( const size_t index )
    {
        return &mFields.at( index );
    }

    virtual void WriteToStream( std::ostream &stream ) const
    {
        for ( typename std::vector< DataType >::const_iterator it = mFields.begin(), end = mFields.end(); it != end; ++it )
        {
            it->WriteToStream( stream );
        }
    }

    virtual void ReadFromStream( std::istream &stream )
    {
        for ( typename std::vector< DataType >::iterator it = mFields.begin(), end = mFields.end(); it != end; ++it )
        {
            it->ReadFromStream( stream );
        }
    }

    virtual Type::Type GetSubType() const
    {
        return SubType;
    }

    virtual size_t Size() const
    {
        size_t size = Util::CalculateVarIntSize( Util::CreateHeader( ( uint32_t )mFields.size(), SubType ) );

        for ( typename std::vector< DataType >::const_iterator it = mFields.begin(), end = mFields.end(); it != end; ++it )
        {
            size += it->Size();
        }

        return size;
    }

    virtual void Resize( const size_t size )
    {
        assert( size >= mFields.size() );
        mFields.resize( size );
    }

    virtual uint32_t GetFieldCount() const
    {
        return ( uint32_t )mFields.size();
    }

protected:

    std::vector< DataType > mFields;
};

typedef RepeatedData< Message, Type::Message > RepeatedMessage;
typedef RepeatedData< StringSerialiseData, Type::String > RepeatedStringData;
typedef RepeatedData< CharSerialiseData, Type::Char > RepeatedCharData;
typedef RepeatedData< WORDSerialiseData, Type::WORD > RepeatedWORDData;
typedef RepeatedData< DWORDSerialiseData, Type::DWORD > RepeatedDWORDData;
typedef RepeatedData< QWORDSerialiseData, Type::QWORD > RepeatedQWORDData;
typedef RepeatedData< VarIntSerialiseData, Type::VarInt > RepeatedVarIntData;


#endif