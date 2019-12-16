#include "BasicType.h"

class Unkown
{
public:
    virtual void* castTo(WORD32 iid) = 0;
};

template<WORD32 iid>
class Interface
{
public:
    enum { EIID = iid};
};