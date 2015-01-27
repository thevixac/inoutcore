/*ProxyNode.cpp
ProxyNode
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "ProxyNode.h"
#include <iostream>
#include <string>

namespace VI = vixac;
namespace IN = VI::inout;



IN::ProxyNode::ProxyNode(IN::NodeStore * store, IN::Node * target)
:IN::Phenode(store->newNode(), store)
{
    store_->addTie(this->root()->nodeId(), IN::Tie(IN::TieType("proxy_target"), target->nodeId(), store_->now()));
}

IN::ProxyNode::ProxyNode(IN::NodeId existingId, IN::NodeStore * store)
: IN::Phenode(store->getNode(existingId), store)
{}

IN::Node * IN::ProxyNode::proxy() const
{
    return store_->getNode(this->root()->getPrimary(IN::TieType("proxy_target")));
}
