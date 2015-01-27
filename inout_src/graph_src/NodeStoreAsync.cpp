/*NodeStoreAsync.cpp
NodeStoreAsync
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "NodeStoreAsync.h"
#include <iostream>
#include <string>

namespace VI = vixac;
namespace IA = VI::ina;
namespace IN = vixac::inout;

IA::NodeAsyncObserver::~NodeAsyncObserver()
{
    if(store_) {
        store_->unsub(this);
    }
}
void IA::NodeStoreAsync::subscribeToNode(IN::NodeId nodeId, IA::NodeAsyncObserver * obs )
{
    obs->bindToStore(this); //so that it can unsubscribe itself.
    if(observers_.find(nodeId) == observers_.end())
    {
        std::set<IA::NodeAsyncObserver *> set;
        set.insert(obs);
        observers_.insert(std::make_pair(nodeId, set));    }
    else
    {
        observers_[nodeId].insert(obs);
    }
    this->handleSubscriptionToNode(nodeId, obs);
}

void IA::NodeStoreAsync::unsub(IA::NodeAsyncObserver * obs)
{
    for(auto it = observers_.begin(), end = observers_.end(); it != end; ++it) {
        auto oit = it->second.find(obs);
        if(oit != it->second.end()) {
            it->second.erase(oit);
        }
    }
}

void IA::NodeStoreAsync::unsubToNode(IN::NodeId nodeId, IA::NodeAsyncObserver * obs )
{
    this->handleUnsubToNode(nodeId , obs);
    abort();
    //TODO WRITE UP
}

std::set<IA::NodeAsyncObserver *> IA::NodeStoreAsync::observersForNodeId(IN::NodeId nodeId)
{

    auto oIt = observers_.find(nodeId);
    if(oIt != observers_.end())
    {
        return oIt->second;
    }
    //TODO return a poointer to the vec or something, copying each time is dumb.
    std::set<IA::NodeAsyncObserver *> v;
    return v;
}

/// heres the spy stuff
void IA::NodeStoreAsync::tieGained(IA::TieChange t)
{
    std::set<IA::NodeAsyncObserver *> set = this->observersForNodeId(t.nodeId);
    for(auto it =set.begin(), end = set.end(); it != end; ++it )
    {
        (*it)->tieGained(t);//TODO occastional crash here when entering chat after going back and forth between views a bit.    
    }
    
    IA::TieChange recT(t.tie.target(), IN::Tie(t.tie.type(), t.nodeId));
    this->tieRecieved(recT);
}

void IA::NodeStoreAsync::tieRemoved(IA::TieChange t)
{
    std::set<IA::NodeAsyncObserver *> set = this->observersForNodeId(t.nodeId);
    for(auto it =set.begin(), end = set.end(); it != end; ++it )
    {
        (*it)->tieRemoved(t);
    }
}
void IA::NodeStoreAsync::tieRecieved(IA::TieChange t)
{
    std::set<IA::NodeAsyncObserver *> set = this->observersForNodeId(t.nodeId);
    for(auto it =set.begin(), end = set.end(); it != end; ++it )
    {
        (*it)->tieRecieved(t);
    }
}

void IA::NodeStoreAsync::intChanged(IA::IntChange t)
{
    

    std::set<IA::NodeAsyncObserver *> set = this->observersForNodeId(t.nodeId);
    for(auto it =set.begin(), end = set.end(); it != end; ++it )
    {
        (*it)->intChanged(t);
    }
}

void IA::NodeStoreAsync::nodeIdChanged(IA::NodeIdChange t)
{
    std::set<IA::NodeAsyncObserver *> set = this->observersForNodeId(t.nodeId);
    for(auto it =set.begin(), end = set.end(); it != end; ++it )
    {
        (*it)->nodeIDChanged(t);
    };
}

void IA::NodeStoreAsync::stringChanged(IA::StringChange t)
{
    std::set<IA::NodeAsyncObserver *> set = this->observersForNodeId(t.nodeId);
    for(auto it =set.begin(), end = set.end(); it != end; ++it )
    {
        (*it)->stringChanged(t);
    }
}




