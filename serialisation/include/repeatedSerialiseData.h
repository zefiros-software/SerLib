#pragma once
#ifndef __REPEATEDSERIALISEDATA_H__
#define __REPEATEDSERIALISEDATA_H__

#include "numSerialiseData.h"
#include "varIntSerialiseData.h"
#include "stringSerialiseData.h"
#include "util.h"

class RepeatedData
    : public ISerialiseData
{
public:

    RepeatedData( Type subType )
        : mFieldCount( 0 ), mSubType( subType )
    {

    }

    virtual void WriteToStream( std::ostream &stream ) const
    {
        VarInt< U64 > header( Util::CreateHeader( mFieldCount, mSubType ) );
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
        VarInt< U64 > header;
        header.ReadFromStream( stream );
        U64 vHeader = header.GetValue();

        assert( Util::GetHeaderType( vHeader ) == mSubType );

        U32 size = mFieldCount + Util::GetHeaderIndex( vHeader );
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

    virtual Type GetType() const
    {
        return Type::Repeated;
    }

    virtual Type GetSubType() const
    {
        return mSubType;
    };

    virtual U32 Size() const
    {
        U32 size = Util::CalculateVarIntSize( Util::CreateHeader( mFieldCount, mSubType ) );

        for ( auto it = mFields.begin(), end = mFields.end(); it != end; ++it )
        {
            size += ( *it )->Size();
        }

        return size;
    }

    ISerialiseData *const GetSerialiseData( U32 index )
    {
        assert( index < mFieldCount );
        return mFields[ index ];
    }

    const ISerialiseData *const GetSerialiseData( U32 index ) const
    {
        assert( index < mFieldCount );
        return mFields[ index ];
    }

    void Resize( const U32 size )
    {
        U32 oldSize = mFields.size();

        if ( size > oldSize )
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
    Type mSubType;
    U32 mFieldCount;

    ISerialiseData *const NewSerialisable( const Type subType )
    {
        switch ( subType )
        {
        case Type::Char:
            return new CharSerialiseData();
            break;

        case Type::DWORD:
            return new DWORDSerialiseData();
            break;

        case Type::DWORDLONG:
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