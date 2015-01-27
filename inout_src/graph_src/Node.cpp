/*Node.cpp
Node
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "Node.h"
#include <algorithm>
#include <iostream>
#include <string>

namespace VI = vixac;
namespace IN = VI::inout;

namespace
{
    
    //functors are fucking wordy.
    struct NotifyGainedTie
    {
        NotifyGainedTie(IN::Node * node, IN::Tie const& tie) :node_(node), tie_(tie){}
        void operator()(IN::NodeObserver * observer)
        {
            if(observer)
            {
                observer->nodeGainedTie(node_, tie_);
            }
        }
        IN::Node * node_;
        IN::Tie tie_;
    };
    struct NotifyRemovedTie
    {
        NotifyRemovedTie(IN::Node * node, IN::Tie const& tie) :node_(node), tie_(tie){}
        void operator()(IN::NodeObserver * observer)
        {
            observer->nodeRemovedTie(node_, tie_);
        }
        IN::Node * node_;
        IN::Tie tie_;
    };
    
    struct NotifyRecievedTie
    {
        NotifyRecievedTie(IN::Node * node, IN::Tie const& tie, IN::NodeId nodeId):node_(node), tie_(tie), nodeId_(nodeId){}
        void operator()(IN::NodeObserver * observer)
        {
            observer->nodeRecievedTie(node_, tie_, nodeId_);
        }
        IN::Node * node_;
        IN::Tie tie_;
        IN::NodeId nodeId_;
    };
    
    struct NotifyMetaChanged
    {
        NotifyMetaChanged(IN::Node * node,std::string const& key, std::string const& value):node_(node), key_(key), value_(value){}
        void operator()(IN::NodeObserver * observer)
        {
            observer->nodeMetaChangedForKey(node_, key_, value_);
        }
        IN::Node * node_;
        std::string key_;
        std::string value_;
    };
    struct NotifyMetaIntChanged
    {
        NotifyMetaIntChanged(IN::Node * node,std::string const& key, int64_t value):node_(node), key_(key), value_(value){}
        void operator()(IN::NodeObserver * observer)
        {
            observer->nodeMetaIntChangedForKey(node_, key_, value_);
        }
        IN::Node * node_;
        std::string key_;
        int64_t value_;
    };
    
    
    struct NotifyChangedNodeId
    {
        NotifyChangedNodeId(IN::Node * node, IN::NodeId nodeId):node_(node), nodeId_(nodeId){}
        void operator()(IN::NodeObserver * observer)
        {
            observer->nodeChangedNodeIdFrom(node_, nodeId_);
        }
        IN::Node * node_;
        IN::NodeId nodeId_;
    };
}

IN::Node::Node(IN::NodeId nodeId, IN::INEpoch epoch) :
nodeId_(nodeId),
createdTime_(epoch)
{}

IN::Node::Node(IN::NodeIdManager * g):
nodeId_(g->nextId())
{}

void IN::Node::removeObserver(IN::NodeObserver * observer)
{
    observers_.erase(std::remove(observers_.begin(), observers_.end(), observer), observers_.end());
}
void IN::Node::addObserver(IN::NodeObserver * observer)
{
    observers_.push_back(observer);
}

void IN::Node::addTie(IN::Tie const& tie, IN::Node * target)
{

    
    this->addTieSilentlyWithCaution(tie , target->nodeId());
    std::for_each(observers_.begin(), observers_.end(), ::NotifyGainedTie(this, tie));
    target->notifyRecievedTie(tie, nodeId_);
}

void IN::Node::addTieSilentlyWithCaution(IN::Tie const& tie, IN::NodeId nodeId)
{

    IN::TieType tieType= tie.type();
    if(ties_.find(tieType) == ties_.end())
    {
        std::vector<IN::Tie> tieVec;
        ties_.insert(std::make_pair(tieType, tieVec));//[tieType] = std::vector<IN::Tie>();
    }
    std::map<IN::TieType, std::vector<IN::Tie> >::iterator it = ties_.find(tieType);
    if(it == ties_.end())
    {
        
        abort();
    }
    it->second.push_back(tie);
}

void IN::Node::notifyRecievedTie(IN::Tie const& tie, IN::NodeId sourceNodeId)
{
    std::for_each(observers_.begin(), observers_.end(), ::NotifyRecievedTie(this, tie, sourceNodeId));
}

void IN::Node::notifyChangedId(IN::NodeId oldId)
{
    std::for_each(observers_.begin(), observers_.end(), ::NotifyChangedNodeId(this, oldId));
}

void IN::Node::addTie(IN::TieType const& tieType, IN::Node * target)
{
    IN::Tie tie(tieType, target->nodeId()); //TODO tie is now redundant
    this->addTie(tie, target);
}

IN::TieTypeIteratorConst IN::Node::getTies(IN::TieType const& tieType) const
{
//    IN::TieTypeIterator it = ties_.find(tieType);
    return ties_.find(tieType);
}


//TODO rewrite this thing.
void IN::Node::incTie(IN::TieType type, IN::Node *target, int64_t value, bool additive)
{
    auto it = ties_.find(type);
    if(it == ties_.end())
    {
        this->addTie(type, target);
        it = ties_.find(type);
    }
    if(it == ties_.end())
    {
        abort();
        
    }
    
    std::vector<IN::Tie>& ties = it->second;
    if(ties.size() ==0)
    {
        abort();
    }
    for(int i=0;i < ties.size(); i++)
    {
        if(ties[i].target() == target->nodeId())
        {
            if(additive)
            {
                ties[i].incSeq(value);
            }
            else
            {
                ties[i].setSeq(value);
            }
            return;
        }
    }
    

    this->addTie(type, target);
    for(int i=0;i < ties.size(); i++)
    {
        if(ties[i].target() == target->nodeId())
        {
            if(additive)
            {
                ties[i].incSeq(value);
            }
            else
            {
                ties[i].setSeq(value);
            }
            return;
        }
    }
    
    abort();
}

int64_t IN::Node::getSeq(IN::TieType type, IN::NodeId target)
{
    auto it = ties_.find(type);
    if(it == ties_.end())
    {
        return 0;
    }
    std::vector<IN::Tie>& ties = it->second;
    for(int i=0;i < ties.size(); i++)
    {
        if(ties[i].target() == target)
        {
            return ties[i].getSeq();
        }
    }
    return 0;
}

//note that the performance of this gets worse as ties get deleted. that shit needs cleaning up.
std::vector<IN::Tie> IN::Node::getActiveTies(IN::TieType const& tieType) const
{
    std::vector<IN::Tie> nodeIds;
    IN::TieTypeIteratorConst  it = this->getTies(tieType);
    if(it == this->tieIteratorEnd())
    {
        return nodeIds;
    }
    std::vector<IN::Tie>const& ties = it->second;
    for(int i=0;i < ties.size();i++)
    {
        IN::Tie const& tie =ties[i];
        if(!tie.deleted())
        {
            nodeIds.push_back(tie);
        }
    }
    return nodeIds;
}

IN::TieTypeIterator IN::Node::getModifiableTies(IN::TieType const& tieType) 
{
    IN::TieTypeIterator it = ties_.find(tieType);
    return it;
}



IN::NodeId IN::Node::getPrimary(IN::TieType const& tieType) const
{
    IN::TieTypeIteratorConst it = this->getTies(tieType);
    if(it == this->tieIteratorEnd() || it->second.size() ==0)
    {
        return this->EMPTY_NODE_ID();
    }
    return it->second.front().target();
}

void IN::Node::flagForDeletion()
{
    this->setString("delete", "true"); //TODO observers will want to hear about this beyond the metachanged no? should be part of the interface
}
void IN::Node::flagTieForDeletion(IN::Tie const& tie)
{
    //we need to find the stored tie and flag that, as this tie here might just be a copy.
    
   // IN::Tie localTie;
    
    IN::TieTypeIterator it = ties_.find(tie.type());
    if(it == ties_.end())
    {
        return;
    }
    std::vector<IN::Tie>& ties = it->second;
    for(int i=0;i < ties.size(); i++)
    {
        IN::Tie& localTie = ties[i];
        if(localTie.target() == tie.target())
        {
            localTie.flagForDeletion();
            std::for_each(observers_.begin(), observers_.end(), ::NotifyRemovedTie(this, localTie));
        }
    }
    
}

//beacuse flaggint doesnt seem to work. fuck that, i aint scared of deleting anymore
void IN::Node::deleteTieOutright(vixac::inout::Tie const& tie)
{
    IN::TieTypeIterator it = ties_.find(tie.type());
    if(it == ties_.end())
    {
        return;
    }
    std::vector<IN::Tie>& ties = it->second;
    for(int i=0;i < ties.size(); i++)
    {
        IN::Tie& localTie = ties[i];
        if(localTie.target() == tie.target())
        {

            localTie.flagForDeletion();
            std::for_each(observers_.begin(), observers_.end(), ::NotifyRemovedTie(this, localTie));
            ties.erase(ties.begin()+i);
            return;
        }
    }
}

std::string IN::Node::getString(std::string const& key) const
{
    std::map<std::string, std::string>::const_iterator it = stringData_.find(key);
    if(it == stringData_.end())
    {
        return  std::string("");
    }
    return it->second;
}

double IN::Node::getFloat(std::string const& key) const
{
    std::map<std::string, double>::const_iterator it = floatData_.find(key);
    if(it == floatData_.end())
    {
        return  0.0;
    }
    return it->second;
}


void IN::Node::setString(std::string const& key, std::string const& value)
{
    std::for_each(observers_.begin(), observers_.end(), ::NotifyMetaChanged(this, key, value));
    stringData_[key] = value;
}

int64_t IN::Node::getInt(std::string const& key) const
{
    std::map<std::string, int64_t>::const_iterator it = intData_.find(key);
    if(it == intData_.end())
    {
        return  0;
    }
    return it->second;
}

void IN::Node::setInt(std::string const& key, int64_t value)
{
    std::for_each(observers_.begin(), observers_.end(), ::NotifyMetaIntChanged(this, key, value));
    intData_[key] = value;
}

void IN::Node::setFloat(std::string const& key, double value)
{
    std::for_each(observers_.begin(), observers_.end(), ::NotifyMetaIntChanged(this, key, value));
    floatData_[key] = value;
}

//TODO have this called in the copy and assignment oeprators.
void IN::Node::unobserve()
{
    observers_.clear();
}
void IN::Node::switchNodeIdWithGreatCaution(IN::NodeId nodeId)
{
    IN::NodeId oldNodeId = nodeId_;
    nodeId_ = nodeId;
    std::for_each(observers_.begin(), observers_.end(), ::NotifyChangedNodeId(this, oldNodeId));
}