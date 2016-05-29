#include "serialisation/tempObject.h"
#include "serialisation/defines.h"

SERIALISATION_INLINE TempObject::TempObject()
    : mTerminatorRead( false )
{
}

SERIALISATION_INLINE Internal::Type::Type TempObject::GetType() const
{
    return Internal::Type::Object;
}

SERIALISATION_INLINE void TempObject::AddData( uint8_t index, ITempData *data )
{
    mTempData.push_back( std::pair< uint8_t, ITempData * >( index, data ) );
}

SERIALISATION_INLINE ITempData *TempObject::TryRemoveData( uint8_t index )
{
    typedef std::vector< std::pair< uint8_t, ITempData * > >::iterator iterator;

    for ( iterator it = mTempData.begin(), end = mTempData.end(); it != end; ++it )
    {
        if ( it->first == index )
        {
            ITempData *data = it->second;
            mTempData.erase( it );
            return data;
        }
    }

    return NULL;
}

SERIALISATION_INLINE size_t TempObject::GetSize()
{
    return mTempData.size();
}

SERIALISATION_INLINE ITempData *TempObject::PopBack()
{
    ITempData *temp = mTempData.back().second;
    mTempData.pop_back();
    return temp;
}

SERIALISATION_INLINE bool TempObject::GetTerminatorRead() const
{
    return mTerminatorRead;
}

SERIALISATION_INLINE void TempObject::SetTerminatorRead()
{
    mTerminatorRead = true;
}

SERIALISATION_INLINE bool TempObject::IsNonEmpty() const
{
    return !mTempData.empty();
}