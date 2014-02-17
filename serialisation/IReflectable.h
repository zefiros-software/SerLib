#pragma once
#ifndef __IREFLECTABLE_H__
#define __IREFLECTABLE_H__

#include "reflect.h"

class IReflectable
{
public:

    virtual void OnReflect( Reflect &reflect ) = 0;
};

#endif