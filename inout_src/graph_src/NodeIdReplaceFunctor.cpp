/*NodeIdReplaceFunctor.cpp
NodeIdReplaceFunctor
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "NodeIdReplaceFunctor.h"
#include <iostream>
#include <string>
namespace VI = vixac;
namespace IN = VI::inout;

void IN::NodeIdReplaceFunctor::swapTempNodeWithMaster(IN::NodeId masterNodeId)
{
    IN::Node * targetNode =store_->getNode(tempNodeId_); // since the registrar is goign to do a getNode, maybe we dont need it here.
    store_->moveNode(targetNode, masterNodeId);
}

IN::NodeId IN::MasterSlaveNodeIdGenerator::get()
{
    IN::NodeId tempId = ++last_;
    IN::NodeIdReplaceFunctor functor(store_, tempId);
    registrar_->registerThisNode(functor);
    return ++last_;
}


IN::NodeId IN::StashGenerator::get()
{
    IN::NodeId next =++last_;
    if(last_ ==high_)
    {
        return IN::Node::EMPTY_NODE_ID();
    }
    if(last_ == threshHold_)
    {
        this->fetchMore();
    }
    return next;
}

void IN::StashGenerator::fetchMore()
{
    IN::NodeIdFetch fetch(this);
    registrar_->fetchAvailableNodeIds(fetch);
}