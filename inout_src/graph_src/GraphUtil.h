//
//  GraphUtil.h
//  inoutcore
//
//  Created by vic on 18/01/2014.
//  Copyright (c) 2014 vixac. All rights reserved.
//

#ifndef inoutcore_GraphUtil_h
#define inoutcore_GraphUtil_h

#include <stdint.h>
#include <time.h>

namespace vixac
{
    namespace inout
    {
        typedef int64_t NodeId;
        typedef int64_t INEpoch;
        
        INEpoch toInEpoch(time_t t);

    }
}

#endif
