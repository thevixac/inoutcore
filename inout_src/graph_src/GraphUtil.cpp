
//
//  GraphUtil.cpp
//  inoutcore
//
//  Created by vic on 31/01/2014.
//  Copyright (c) 2014 vixac. All rights reserved.
//

#include "GraphUtil.h"

namespace IN = vixac::inout;

IN::INEpoch IN::toInEpoch(time_t t)
{
    return INEpoch(t);
}

