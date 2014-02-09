#pragma once
#ifndef __ISERIALIZABLE_H__
#define __ISERIALIZABLE_H__

class Message;

class ISerialisable
{
public:

    virtual void OnSerialise( Message &message ) = 0;
};

#endif