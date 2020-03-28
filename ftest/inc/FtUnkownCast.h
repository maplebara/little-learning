#pragma once

#include "UnkownCast.h"
#include <iostream>
#include <memory>

using namespace std;

class A : public Interface<0x20191017>
{
public:
    void a() {}
};

class Root : public Unkown, 
             A
{
public:
    void* castTo(U32 iid)
    {
        switch(iid)
        {
            case A::EIID: return static_cast<A*>(this);
        }
    }
};

class RootFactory
{
public:
    static Unkown* createRoot()
    {
        return new Root;
    }
};

int main()
{
    Unkown* root = RootFactory::createRoot();
}
