#pragma once
#ifndef __SERIALISEDATA_H__
#define __SERIALISEDATA_H__

#include <iostream>
#include "types.h"

class ISerialiseData
{
public:

    virtual void ReadFromStream( std::istream &stream ) = 0;
    virtual void WriteToStream( std::ostream &stream ) const = 0;

    virtual Type::Type GetType() const = 0;

    virtual size_t Size() const = 0;
};

#endif