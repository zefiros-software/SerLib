#include "serialisation/bufferedStreamWriter.h"

SERIALISATION_INLINE BufferedStreamWriter::BufferedStreamWriter( const std::string &fileName )
    : mStreamWriter( fileName ),
      mWriteIndex( 0 )
{

}

SERIALISATION_INLINE BufferedStreamWriter::BufferedStreamWriter( std::ofstream &stream )
    : mStreamWriter( stream ),
      mWriteIndex( 0 )
{

}

SERIALISATION_INLINE BufferedStreamWriter::BufferedStreamWriter( std::fstream &stream )
    : mStreamWriter( stream ),
      mWriteIndex( 0 )
{

}

SERIALISATION_INLINE BufferedStreamWriter::BufferedStreamWriter( std::ostream &stream )
    : mStreamWriter( stream ),
      mWriteIndex( 0 )
{

}

SERIALISATION_INLINE BufferedStreamWriter::~BufferedStreamWriter()
{
    Close();
}

SERIALISATION_INLINE void BufferedStreamWriter::ClearBuffer()
{
    mStreamWriter.WriteBytes( mWriteBuffer, mWriteIndex );
    mWriteIndex = 0;

    mStreamWriter.ClearBuffer();
}

SERIALISATION_INLINE void BufferedStreamWriter::Close()
{
    ClearBuffer();

    mStreamWriter.Close();
}