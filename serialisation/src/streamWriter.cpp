#include "serialisation/streamWriter.h"

SERIALISATION_INLINE StreamWriter::StreamWriter( const std::string &fileName )
    : mFileStream( fileName.c_str(), std::ifstream::binary | std::ifstream::out ),
      mStream( &mFileStream )
{
    assert( mFileStream.is_open() && "File does not exist" );
}

SERIALISATION_INLINE StreamWriter::StreamWriter( std::ofstream &stream )
    : mStream( &stream )
{
    assert( mStream->flags() & std::ios::binary );
}

SERIALISATION_INLINE StreamWriter::StreamWriter( std::fstream &stream )
    : mStream( &stream )
{
    assert( ( stream.flags() & std::ios::out ) && "Not an input stream" );
    assert( ( stream.flags() & std::ios::binary ) && "File stream is not in binary mode" );
}

SERIALISATION_INLINE StreamWriter::StreamWriter( std::ostream &stream )
    : mStream( &stream )
{

}

SERIALISATION_INLINE StreamWriter::~StreamWriter()
{
    Close();
}

SERIALISATION_INLINE void StreamWriter::ClearBuffer()
{
    mStream->flush();
}

SERIALISATION_INLINE void StreamWriter::Close()
{
    ClearBuffer();

    if ( mFileStream.is_open() )
    {
        mFileStream.close();
    }
}

SERIALISATION_INLINE void StreamWriter::WriteBlock( const char *const firstByte, size_t byteCount )
{
    WriteBytes( firstByte, byteCount );
}

SERIALISATION_INLINE void StreamWriter::WriteSize( size_t size )
{
    uint8_t bufferIndex;

    for ( bufferIndex = 0; size >= 0x80; size >>= 7, bufferIndex++ )
    {
        mVarIntBuffer[bufferIndex] = static_cast<uint8_t>( ( size & 0x7F ) | 0x80 );
    }

    mVarIntBuffer[bufferIndex] = static_cast<uint8_t>( size );

    WriteBytes( reinterpret_cast<char *>( mVarIntBuffer ), ++bufferIndex );
}