/*Phenode.cpp
Phenode
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "Phenode.h"
#include <iostream>
#include <string>

namespace VI = vixac;
namespace IN = VI::inout;



IN::Phenode::Phenode(IN::Node * node, IN::NodeStore * store)
:store_(store),
//parentId_(node->nodeId()),
root_(node),
isValid_(true)
{
    //TODO unpack this mutha
}

void IN::Phenode::nodeChanged(IN::NodeId nodeId)
{
}

void IN::Phenode::nodeAdded(IN::NodeId nodeId)
{
}

void IN::Phenode::childRemovedFrom(IN::NodeId nodeId)
{
}
