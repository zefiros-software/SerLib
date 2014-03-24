#pragma once
#ifndef __SERIALISATION_ABSTRACTPRIMITIVEDATA_H__
#define __SERIALISATION_ABSTRACTPRIMITIVEDATA_H__

#include "interface/ISerialiseData.h"

class AbstractPrimitiveData
    : public ISerialiseData
{
public:

    virtual void Store( std::string &value, Mode::Mode mode ) = 0;

    virtual void Store( uint8_t &value, Mode::Mode mode ) = 0;

    virtual void Store( uint16_t &value, Mode::Mode mode ) = 0;

    virtual void Store( uint32_t &value, Mode::Mode mode ) = 0;

    virtual void Store( uint64_t &value, Mode::Mode mode ) = 0;

    virtual void Store( int8_t &value, Mode::Mode mode ) = 0;

    virtual void Store( int16_t &value, Mode::Mode mode ) = 0;

    virtual void Store( int32_t &value, Mode::Mode mode ) = 0;

    virtual void Store( int64_t &value, Mode::Mode mode ) = 0;

    virtual void Store( float &value, Mode::Mode mode ) = 0;

    virtual void Store( double &value, Mode::Mode mode ) = 0;
};

#endif