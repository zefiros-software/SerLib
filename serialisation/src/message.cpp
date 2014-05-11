/**
 * Copyright (c) 2014 Mick van Duijn, Koen Visscher and Paul Visscher
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "interface/abstractSerialiser.h"
#include "interface/ISerialisable.h"

#include "repeatedData.h"
#include "varIntData.h"
#include "defines.h"
#include "message.h"

Message::Message( Mode::Mode mode /*= Mode::Serialise */ )
    : mMode( mode ),
      mMemberCount( 0 ),
      mFlags( 0x00 )
{
    mIndexes.reserve( 10 );
    mSerialisables.reserve( 10 );
}

Message::Message( ISerialisable *const serialisable, Mode::Mode mode /*= Mode::Serialise */ )
    : mMode( mode ),
      mMemberCount( 0 ),
      mFlags( 0x00 )
{
    mIndexes.reserve( 10 );
    mSerialisables.reserve( 10 );
    Store( serialisable );
}

Message::~Message()
{
    for ( std::vector< uint32_t >::iterator it = mIndexes.begin(), end = mIndexes.end(); it != end; ++it )
    {
        mSerialisables[ *it ]->Dispose();
    }
}

void Message::SetMode( Mode::Mode mode )
{
    mMode = mode;
}

Mode::Mode Message::GetMode() const
{
    return mMode;
}

uint32_t Message::GetFlags() const
{
    return mFlags;
}

void Message::SetFlags( const uint32_t flags )
{
    mFlags = flags;
}

Internal::Type::Type Message::GetType() const
{
    return Internal::Type::Variable;
}

void Message::Store( ISerialisable *const value, const uint32_t index, const uint32_t flags /*= 0x00 */ )
{
    ISerialiseData *data = FindSerialisable( index );

    if ( !data )
    {
        data = CreateDataType< Message >( flags );
        InsertSerialiseDataAt( data, index );
    }

    static_cast< Message * >( data )->Store( value, mMode );
}

void Message::Store( std::string &value, const uint32_t index, const uint32_t flags/*= 0x00 */ )
{
    Store< SerialiseData< std::string > >( value, index, flags );
}

void Message::Store( uint8_t &value, const uint32_t index, const uint32_t flags /*= 0x00 */ )
{
    Store< SerialiseData< uint8_t > >( value, index, flags );
}

void Message::Store( uint16_t &value, const uint32_t index, const uint32_t flags /*= 0x00 */ )
{
    Store< SerialiseData< uint16_t > >( value, index, flags );
}

void Message::Store( uint32_t &value, const uint32_t index, const uint32_t flags /*= 0x00 */ )
{
    Store< SerialiseData< uint32_t > >( value, index, flags );
}

void Message::Store( uint64_t &value, const uint32_t index, const uint32_t flags /*= 0x00 */ )
{
    Store< SerialiseData< uint64_t > >( value, index, flags );
}

void Message::Store( int8_t &value, const uint32_t index, const uint32_t flags /*= 0x00 */ )
{
    Store< SerialiseData< int8_t > >( value, index, flags );
}

void Message::Store( int16_t &value, const uint32_t index, const uint32_t flags /*= 0x00 */ )
{
    Store< SerialiseData< int16_t > >( value, index, flags );
}

void Message::Store( int32_t &value, const uint32_t index, const uint32_t flags /*= 0x00 */ )
{
    Store< SerialiseData< int32_t > >( value, index, flags );
}

void Message::Store( int64_t &value, const uint32_t index, const uint32_t flags /*= 0x00 */ )
{
    Store< SerialiseData< int64_t > >( value, index, flags );
}

void Message::Store( float &value, const uint32_t index, const uint32_t flags /*= 0x00 */ )
{
    Store< SerialiseData< float > >( value, index, flags );
}

void Message::Store( double &value, const uint32_t index /*= 0*/, const uint32_t flags /*= 0x00 */ )
{
    Store< SerialiseData< double > >( value, index, flags );
}

void Message::Store( ISerialisable *const value )
{
    value->SERIALISATION_CUSTOM_INTERFACE( *this );
}

void Message::Store( ISerialisable *const value, const Mode::Mode mode )
{
    mMode = mode;
    Store( value );
}

uint32_t Message::Count( const uint32_t index )
{
    ISerialiseData *const data = FindSerialisable( index );

    if ( data->GetType() == Internal::Type::Repeated )
    {
        return static_cast< AbstractRepeatedData *const >( data )->Count();
    }

    return 1;
}

uint32_t Message::GetMemberCount() const
{
    return mMemberCount;
}

void Message::CreateRepeated( Type::Type type, uint32_t size, const uint32_t index, const uint32_t flags /*= 0x00 */ )
{
    if ( mMode == Mode::Serialise )
    {
        ISerialiseData *data = FindSerialisable( index );

        if ( !data )
        {
            switch ( type )
            {
            case Type::Variable:
                data = CreateDataType< RepeatedData< Message > >( flags );
                break;

            case Type::String:
                data = CreateDataType< RepeatedData< SerialiseData< std::string > > >( flags );
                break;

            case Type::UInt8:
                data = CreateDataType< RepeatedData< SerialiseData< uint8_t > > >( flags );
                break;

            case Type::UInt16:
                data = CreateDataType< RepeatedData< SerialiseData< uint16_t > > >( flags );
                break;

            case Type::UInt32:
                data = CreateDataType< RepeatedData< SerialiseData< uint32_t > > >( flags );
                break;

            case Type::UInt64:
                data = CreateDataType< RepeatedData< SerialiseData< uint64_t > > >( flags );
                break;

            case Type::SInt8:
                data = CreateDataType< RepeatedData< SerialiseData< int8_t > > >( flags );
                break;

            case Type::SInt16:
                data = CreateDataType< RepeatedData< SerialiseData< int16_t > > >( flags );
                break;

            case Type::SInt32:
                data = CreateDataType< RepeatedData< SerialiseData< int32_t > > >( flags );
                break;

            case Type::SInt64:
                data = CreateDataType< RepeatedData< SerialiseData< int64_t > > >( flags );
                break;

            case Type::Float:
                data = CreateDataType< RepeatedData< SerialiseData< float > > >( flags );
                break;

            case Type::Double:
                data = CreateDataType< RepeatedData< SerialiseData< double > > >( flags );
                break;

            default:
                data = CreateDataType< RepeatedData< VarIntData > >( flags );
                break;
            }

            InsertSerialiseDataAt( data, index );
        }

        static_cast< AbstractRepeatedData * >( data )->Resize( size );
    }
}

void Message::StoreRepeated( ISerialisable *const value, const uint32_t index, const uint32_t repeatedIndex )
{
    ISerialiseData *data = FindSerialisable( index );

    assert( data->GetType() == Internal::Type::Repeated );

    AbstractRepeatedData *rdata = static_cast< AbstractRepeatedData * >( data );
    static_cast< Message * >( rdata->GetSerialisable( repeatedIndex ) )->Store( value, mMode );
}

void Message::StoreRepeated( std::string &value, const uint32_t index, const uint32_t repeatedIndex )
{
    StoreRepeatedV( value, index, repeatedIndex );
}

void Message::StoreRepeated( uint8_t &value, const uint32_t index, const uint32_t repeatedIndex )
{
    StoreRepeatedV( value, index, repeatedIndex );
}

void Message::StoreRepeated( uint16_t &value, const uint32_t index, const uint32_t repeatedIndex )
{
    StoreRepeatedV( value, index, repeatedIndex );
}

void Message::StoreRepeated( uint32_t &value, const uint32_t index, const uint32_t repeatedIndex )
{
    StoreRepeatedV( value, index, repeatedIndex );
}

void Message::StoreRepeated( uint64_t &value, const uint32_t index, const uint32_t repeatedIndex )
{
    StoreRepeatedV( value, index, repeatedIndex );
}

void Message::StoreRepeated( int8_t &value, const uint32_t index, const uint32_t repeatedIndex )
{
    StoreRepeatedV( value, index, repeatedIndex );
}

void Message::StoreRepeated( int16_t &value, const uint32_t index, const uint32_t repeatedIndex )
{
    StoreRepeatedV( value, index, repeatedIndex );
}

void Message::StoreRepeated( int32_t &value, const uint32_t index, const uint32_t repeatedIndex )
{
    StoreRepeatedV( value, index, repeatedIndex );
}

void Message::StoreRepeated( int64_t &value, const uint32_t index, const uint32_t repeatedIndex )
{
    StoreRepeatedV( value, index, repeatedIndex );
}

void Message::StoreRepeated( float &value, const uint32_t index, const uint32_t repeatedIndex )
{
    StoreRepeatedV( value, index, repeatedIndex );
}

void Message::StoreRepeated( double &value, const uint32_t index, const uint32_t repeatedIndex )
{
    StoreRepeatedV( value, index, repeatedIndex );
}

void Message::SerialiseTo( AbstractSerialiser *const serialiser )
{
    serialiser->Serialise( this );
}

void Message::Dispose()
{
    PoolHolder::Get().GetPool< Message >().Dispose( this );
}
