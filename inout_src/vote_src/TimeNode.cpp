/*TimeNode.cpp
TimeNode
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "TimeNode.h"
#include <iostream>
#include <string>

namespace VI = vixac;
namespace IN = VI::inout;


IN::TimeNode::TimeNode(IN::NodeStore * store, IN::INEpoch epoch, IN::TimeResolution::Type res,  int64_t durationInSeconds )
:IN::Phenode(store->newNode(), store)
{
    
    this->setEpoch(epoch);
    this->root()->setInt("resolution", res);
    this->root()->setInt("duration_seconds", durationInSeconds);
    
}

IN::TimeNode::TimeNode(IN::NodeId existingId, IN::NodeStore * store)
: IN::Phenode(store->getNode(existingId), store)
{}

IN::INEpoch IN::TimeNode::epoch() const
{
    return IN::INEpoch(this->root()->getInt("epoch_start"));
}

void IN::TimeNode::setEpoch(IN::INEpoch epoch) const
{
    this->root()->setInt("epoch_start", epoch);
}

int64_t IN::TimeNode::durationInSeconds() const
{
    return this->root()->getInt("duration_seconds");
}

IN::TimeResolution::Type IN::TimeNode::resolution() const
{
    return (IN::TimeResolution::Type) this->root()->getInt("resolution");
}