/*NodeGenAsync.h
NodeGenAsync
 copyright Vixac Ltd. All Rights Reserved 
*/

#ifndef INCLUDED_NODEGENASYNC
#define INCLUDED_NODEGENASYNC


#include <vector>
#include "Node.h"
#include "AsyncFunctor.h"
namespace vixac
{ 
    namespace ina
    { 
        class NodeGenAsync;
        //
        class AsyncGetFunc;
    }
}

class vixac::ina::NodeGenAsync
{
public:
    virtual void a_get(size_t quantity, vixac::ina::NodeIdVecFunc   func) =0;
    virtual ~NodeGenAsync(){}
};

#endif
