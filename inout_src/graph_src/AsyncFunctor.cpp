//
//  AsyncFunctor.cpp
//  inoutcore
//
//  Created by vic on 28/05/2014.
//  Copyright (c) 2014 vixac. All rights reserved.
//

#include "AsyncFunctor.h"

#include <set>

namespace IN = vixac::inout;
namespace IA = vixac::ina;
    

//TODO test
IA::MetaMap IA::metaVecToMap(IA::MetaVec const& vec)
{
    vixac::ina::MetaMap map;
    std::set<vixac::inout::NodeId> nodeIds;
    for(int i=0;i < vec.size(); i++)
    {
        vixac::ina::Meta const& meta = vec[i];
        vixac::inout::NodeId nodeId = meta.rootId();
        if(nodeIds.insert(nodeId).second) { //first time we're seeing this nodeId.
            vixac::ina::KeyMetaMap keyMetas;
            keyMetas[meta.key_] = meta; //this means that each meta doesnt need to hold a nodeId or a key anymore, but oh well.
            map[nodeId] =keyMetas;
        }
        else {
            map[nodeId][meta.key_] = meta;
        }
    }
    return map;
}
