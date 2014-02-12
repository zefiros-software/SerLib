#pragma once
#ifndef __TYPES_H__
#define __TYPES_H__

#include "macro.h"

#include <stdint.h>

#ifndef CPP11

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;

#endif

namespace Type
{
    // underlying type is uint8_t
    enum Type
    {
        Message   = 0x00,
        String    = 0x01,
        Char      = 0x02,
        WORD      = 0x03,
        DWORD     = 0x04,
        QWORD = 0x05,
        VarInt    = 0x06,
        Repeated  = 0x07
    };
}

namespace Mode
{


    enum Mode
    {
        Serialise = 0x00,
        Deserialise = 0x01
    };
}

#endif