#pragma once
#ifndef __STRINGSERIALISEDATA_H__
#define __STRINGSERIALISEDATA_H__

#include "serialiseData.h"

class StringSerialiseData
    : public ISerialiseData
{
public:

    void Store( std::string &str, Mode::Mode mode );

    virtual Type::Type GetType() const;

    virtual size_t Size() const;

    virtual void ReadFromStream( std::istream &stream );

    virtual void WriteToStream( std::ostream &stream ) const;

private:

    std::string mString;
};

#endif