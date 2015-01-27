/*NodeStore.cpp
NodeStore
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "NodeStore.h"
#include "GraphUtil.h"
#include "NodeIdReplaceFunctor.h"
#include <time.h>
#include <iostream>
#include <string>
#include <cstdlib>
namespace VI = vixac;
namespace IN = VI::inout;




void IN::NodeStore::attachObserverToAll(IN::NodeObserver * observer)
{
    if(omniObserver_)
    {
        std::cout<<"TODO HANDLE MULTIPLE OMINI OBSERVERS" <<std::endl;
        abort();
    }
    omniObserver_ = observer;
    for(std::map<IN::NodeId, IN::Node>::iterator it = nodes_.begin(), end = nodes_.end(); it != end; ++it)
    {
        it->second.addObserver(omniObserver_);
    }
}

IN::Node * IN::NodeStore::getNode(IN::NodeId nodeId)
{
    std::map<IN::NodeId, IN::Node *>::iterator it = nodeLookup_.find(nodeId);
    if(it != nodeLookup_.end())
    {
        return it->second;
    }
    return NULL;
}

IN::Node * IN::NodeStore::newNode()
{
    IN::NodeId nodeId= this->generator_ ? this->generator_->get() : this->highestId() +1;
    if(nodeId == IN::Node::EMPTY_NODE_ID())
    {
        return NULL;
    }
    IN::Node n(nodeId, this->now());
    return this->injectExistingNode(n);
}

void IN::NodeStore::addTie(IN::NodeId from, IN::Tie tie)
{
    std::map<NodeId, Node *>::iterator fromIt = nodeLookup_.find(from);
    std::map<NodeId, Node *>::iterator toIt = nodeLookup_.find(tie.target());
    if(fromIt == nodeLookup_.end() || toIt == nodeLookup_.end())
    {
        std::cout<<"fail adding tie" <<std::endl;
        abort();
        //TODO an error pls
        return; 
    }
    fromIt->second->addTie(tie, (toIt->second));
    
}

IN::INEpoch IN::NodeStore::now()
{
    time_t t;
    time(&t);
    return IN::toInEpoch(t);
    
}
void IN::NodeStore::swapNodeIds(vixac::inout::NodeId first, vixac::inout::NodeId second)
{

    //TODO not sure how this relates to nodelookup
    IN::Node firstNode = nodes_[first];
    IN::Node secondNode = nodes_[second];
    //TODO lock here?
    nodes_[first] = secondNode;
    nodes_[second] = firstNode;
    nodes_[first].switchNodeIdWithGreatCaution(first); //important this happens after, otherwise the phenospies won't know whats going on.
    nodes_[second].switchNodeIdWithGreatCaution(second);
    
}
void IN::NodeStore::moveNode(IN::Node * node, IN::NodeId nodeId)
{
    if(this->getNode(nodeId) != NULL)
    {
        std::cout<<"ERROR, trying to move node to a location thats already taken " << nodeId <<std::endl;
        abort();
    }
    //the old nodeLookup entry is still there, so the old nodeid can be used, just about.
    nodeLookup_[nodeId] = node;
    node->switchNodeIdWithGreatCaution(nodeId);
}

IN::Node * IN::NodeStore::injectExistingNode(vixac::inout::Node const& node)
{
    
    IN::NodeId nodeId = node.nodeId();
    IN::Node * storedNode = &(nodes_[nodeId ] = node); // you love it. when you insert you get a ref to what you inserted.

  //  IN::Node * storedNode = this->getNode(node.nodeId());
    nodeLookup_[nodeId] = storedNode;
    if(omniObserver_)
    {
        storedNode->addObserver(omniObserver_);
    }
    return storedNode;
}
