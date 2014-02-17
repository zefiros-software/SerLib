#pragma once
#ifndef __REFLECT_H__
#define __REFLECT_H__

#include "types.h"

#include <assert.h>

#ifdef CPP11
#include <unordered_map>
#else
#include <map>
#endif

class Reflect
{
public:

    void Set( const uint32_t index, const std::string &name );

    const std::string &Get( const uint32_t index ) const;

    bool Exists( const uint32_t index ) const;

private:

#ifdef CPP11
    typedef std::unordered_map< uint32_t, std::string > Map;
#else
    typedef std::map< uint32_t, std::string > Map;
#endif


    Map mNames;
};

#endif