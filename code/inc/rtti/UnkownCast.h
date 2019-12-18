#include "BasicType.h"

class Unkown
{
public:
    virtual void* castTo(U32 iid) = 0;
};

template<U32 iid>
class Interface
{
public:
    enum { EIID = iid};
};