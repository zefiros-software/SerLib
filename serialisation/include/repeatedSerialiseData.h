#pragma once
#ifndef __REPEATEDSERIALISEDATA_H__
#define __REPEATEDSERIALISEDATA_H__

#include "numSerialiseData.h"
#include "varIntSerialiseData.h"
#include "stringSerialiseData.h"
#include "util.h"
#include <assert.h>

class RepeatedData
    : public ISerialiseData
{
public:

    RepeatedData( Type::Type subType )
        : mFieldCount( 0 ), mSubType( subType )
    {

    }

    virtual void WriteToStream( std::ostream &stream ) const
    {
        VarInt< uint64_t > header( Util::CreateHeader( mFieldCount, mSubType ) );
        header.WriteToStream( stream );

        for ( auto it = mFields.begin(), end = mFields.begin() + mFieldCount; it != end; ++it )
        {
            ISerialiseData *const data = *it;
            assert( data->GetType() == mSubType );

            if ( data->GetType() == mSubType )
            {
                data->WriteToStream( stream );
            }
        }
    }

    virtual void ReadFromStream( std::istream &stream )
    {
        VarInt< uint64_t > header;
        header.ReadFromStream( stream );
        uint64_t vHeader = header.GetValue();

        assert( Util::GetHeaderType( vHeader ) == mSubType );

        uint32_t size = mFieldCount + Util::GetHeaderIndex( vHeader );
        mSubType = Util::GetHeaderType( vHeader );
        mFields.resize( size );

        for ( auto it = mFields.begin() + mFieldCount, end = mFields.begin() + size; it != end; ++it )
        {
            ISerialiseData *data = *it;
            assert( data->GetType() == mSubType );

            if ( data->GetType() != mSubType )
            {
                delete data;
                data = NewSerialisable( mSubType );
                *it = data;
            }

            data->ReadFromStream( stream );
        }

        mFieldCount = size;
    }

    virtual Type::Type GetType() const
    {
        return Type::Repeated;
    }

    virtual Type::Type GetSubType() const
    {
        return mSubType;
    };

    virtual uint32_t Size() const
    {
        uint32_t size = Util::CalculateVarIntSize( Util::CreateHeader( mFieldCount, mSubType ) );

        for ( auto it = mFields.begin(), end = mFields.end(); it != end; ++it )
        {
            size += ( *it )->Size();
        }

        return size;
    }

    ISerialiseData *const GetSerialiseData( uint32_t index )
    {
        assert( index < mFieldCount );
        return mFields.at( index );
    }

    const ISerialiseData *const GetSerialiseData( uint32_t index ) const
    {
        assert( index < mFieldCount );
        return mFields.at( index );
    }

    void Resize( const uint32_t size )
    {
        if ( size > mFields.size() )
        {
            mFields.resize( size );

            for ( auto it = mFields.begin(), end = mFields.end(); it != end; ++it )
            {
                *it = NewSerialisable( mSubType );
            }
        }

        mFieldCount = size;
    }

    size_t GetFieldCount() const
    {
        return mFieldCount;
    }

protected:

    std::vector< ISerialiseData * > mFields;
    Type::Type mSubType;
    uint32_t mFieldCount;

    ISerialiseData *const NewSerialisable( const Type::Type subType )
    {
        switch ( subType )
        {
        case Type::Char:
            return new CharSerialiseData();
            break;

        case Type::DWORD:
            return new DWORDSerialiseData();
            break;

        case Type::QWORD:
            return new DWORDLONGSerialiseData();
            break;

        case Type::Message:
            return new Message();
            break;

        case Type::String:
            return new StringSerialiseData();
            break;

        case Type::VarInt:
            return new VarIntSerialiseData();
            break;

        case Type::WORD:
            return new WORDSerialiseData();
            break;
        }
    }
};

#endif