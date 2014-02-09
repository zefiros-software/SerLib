#pragma once
#ifndef __TYPES_H__
#define __TYPES_H__

#include <stdint.h>

typedef uint8_t  U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;

typedef int8_t  S8;
typedef int16_t S16;
typedef int32_t S32;
typedef int64_t S64;

typedef float F32;
typedef double F64;

enum class Type : U8
{
    Message = 0x00,
    String = 0x01,
    Char = 0x02,
    WORD = 0x03,
    DWORD = 0x04,
    DWORDLONG = 0x05,
    VarInt = 0x06,
    Repeated = 0x07
};

enum class Flags : U32
{
    VarInt = 0x01
};

enum class Mode : U8
{
    Serialise = 0x00,
    Deserialise = 0x01
};

#endif