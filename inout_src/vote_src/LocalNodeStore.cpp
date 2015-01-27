/*LocalNodeStore.cpp
LocalNodeStore
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "LocalNodeStore.h"
#include "NodeAsync.h"
#include <iostream>
#include <string>
#include <thread>
#include <algorithm>
namespace VI = vixac;
namespace IA = VI::ina;
namespace IN = vixac::inout;


namespace
{
    struct SortBySeq
    {
        
        SortBySeq(IA::LocalNodeStore * store, IN::Node * base, IN::TieType type):store_(store), base_(base), type_(type){}
        
        bool operator()(IN::NodeId  lhs, IN::NodeId rhs)
        {
            
            int64_t lseq = base_->getSeq(type_, lhs);
            int64_t rseq = base_->getSeq(type_, rhs);
            return lseq <rseq;
            
            
        }
        IN::Node * base_;
        IA::LocalNodeStore *store_;
        IN::TieType type_;
        
    };

}
/*
void IA::LocalNodeStore::getSequence(IA::SequenceRequest r, IA::NodeIdVecFunc f)
{
   
    //std::cout<<"WARN getSequence here not fully implmenented" <<std::endl;
    
    
    this->getNode(r.nodeId, [&](IN::Node * base) {
        this->getActiveTies(r.nodeId, r.type,[&](std::vector<IN::NodeId> nodes) {
            

            
            
            std::vector<IN::NodeId> res;
            std::map<int64_t, IN::NodeId> seqMap;
            if(r.quantity ==0)
            {
                r.quantity = INT64_MAX;
            }
            if(r.sort == IA::SortTypes::Creation)
            {
                std::cout<<"TODO IMPLEMENT"<<std::endl;
                f(nodes);
                return;
            }
            else if( r.sort == IA::SortTypes::Update)
            {
                std::cout<<"TODO OIMPLEMNTNE UPDATE " <<std::endl;
                f(nodes);
                return;
            }
            else if(r.sort == IA::SortTypes::Sequence)
            {
                 ::SortBySeq seq(this,base, r.type);
                std::sort(nodes.begin(), nodes.end(), seq);
            }
    
            size_t range =r.quantity < nodes.size() ? (size_t)r.quantity : nodes.size();
            if(!r.asc)
            {
                std::reverse(nodes.begin(), nodes.end()); //TODO V SLOW. sorts entire fucking thing
            }
          
            for(int i=0;i < range; i++)
            {
                res.push_back(nodes[i]);
            }
            f(res);
         });
    });

}
*/



IA::LocalNodeStore::LocalNodeStore()
{
//    this->new
    IN::Node n(0,0);
    nodeMap_[0] = n; //THE GLOBAL NODE, so lists can be kept
    
    
    
}

void IA::LocalNodeStore::newNode(IN::NodeType type, IA::NodeIdFunc  f)
{
    IN::NodeId nodeId =  nodeMap_.empty() ? 1 : nodeMap_.rbegin()->first +1;
    IN::Node n(nodeId,0);//TOOD doesnt use nodetype
    
    nodeMap_[nodeId] = n;
    f(nodeId);
}


// so i guess another implmenetion would need to cache the damnt hing and return a ptr to that. or at least a ptr to something that can build itself?
//no its a ptr to something syncronous. this doesnt work.
void IA::LocalNodeStore::getNode(vixac::inout::NodeId nodeId, IA::NodePtrFunc  f)
{
    if(nodeMap_.find(nodeId) == nodeMap_.end())
    {
        
        f(NULL);
    }
    else
    {
       f(&(nodeMap_[nodeId]));
    }
}


void IA::LocalNodeStore::addTie(vixac::inout::NodeId nodeId, vixac::inout::Tie tie, IA::DoneFunc  f)
{
    this->getNode(nodeId, [=](IN::Node *  base) {
        if(base == NULL)
        {
            abort();
        }
        this->getNode(tie.target(), [=](IN::Node * target) {
            if(target == NULL)
            {
                abort();
            }
            base->addTie(tie, target);
            
            TieChange t(nodeId, tie);
            this->tieGained(t);
            f();
        });
    });
}

void IA::LocalNodeStore::removeTie(IN::NodeId nodeId, IN::Tie tie, IA::DoneFunc f)
{
    this->getNode(nodeId, [=](IN::Node *  base) {
        if(base == NULL)
        {
            abort();
        }
        this->getNode(tie.target(), [=](IN::Node * target) {
            if(target == NULL)
            {
                abort();
            }
            base->deleteTieOutright(tie);
            TieChange t(nodeId, tie);
            this->tieRemoved(t);
            f();
        });
    });
    
}

void IA::LocalNodeStore::injectNode(vixac::inout::Node const& n, IA::DoneFunc  f)
{
    nodeMap_[n.nodeId()] = n;
}


void IA::LocalNodeStore::setStringMeta( vixac::inout::NodeId nodeId, std::string const& key, std::string const& value, IA::DoneFunc f)
{
    this->getNode(nodeId, [=](IN::Node * base) {
        if(base == NULL)
        {
            abort();
        }
        base->setString(key, value);
        
        StringChange c(nodeId, key, value);
        this->stringChanged(c);
        f();

    });
}

void IA::LocalNodeStore::setFloatMeta( vixac::inout::NodeId nodeId, std::string const& key, double value, IA::DoneFunc f)
{
    this->getNode(nodeId, [=](IN::Node * base) {
        if(base == NULL)
        {
            abort();
        }
        base->setFloat(key, value);
        f();
    });
    
}


void IA::LocalNodeStore::setIntMeta(vixac::inout::NodeId nodeId, std::string const& key, int64_t value, IA::DoneFunc f)
{
    this->getNode(nodeId, [&](IN::Node * base) {
        if(base == NULL)
        {
            abort();
        }
        base->setInt(key, value);
        f();
    });
}

void IA::LocalNodeStore::getStringMeta(vixac::inout::NodeId nodeId, std::string const& key, IA::StringFunc f)
{
    this->getNode(nodeId, [&](IN::Node * base) {
        if(base == NULL)
        {
            abort();
        }
        f(base->getString(key));
    });
}
void IA::LocalNodeStore::getFloatMeta(vixac::inout::NodeId nodeId, std::string const& key, IA::FloatFunc f)
{
    this->getNode(nodeId, [&](IN::Node * base) {
        if(base == NULL)
        {
            abort();
        }
        f(base->getFloat(key));
    });
}

void IA::LocalNodeStore::getIntMeta(vixac::inout::NodeId nodeId, std::string const& key, IA::IntFunc f)
{
    this->getNode(nodeId, [=](IN::Node * base) {
        if(base == NULL)
        {
            abort();
        }
        f(base->getInt(key));
    });
}
void IA::LocalNodeStore::getPrimary(vixac::inout::NodeId nodeId, vixac::inout::TieType type, IA::NodeIdFunc f)
{
    this->getNode(nodeId, [&](IN::Node * base) {
        if(base == NULL)
        {
            abort();
        }
        f(base->getPrimary(type));
    });
}

void IA::LocalNodeStore::getActiveTies(vixac::inout::NodeId nodeId, vixac::inout::TieType type, IA::NodeIdVecFunc f)
{
    this->getNode(nodeId, [&](IN::Node * base) {
        std::vector<IN::Tie> ties =base->getActiveTies(type);
        std::vector<IN::NodeId> nodeIds;
        for (int i=0;i < ties.size(); i++)
        {
            nodeIds.push_back(ties[i].target());
        }
        f(nodeIds);
    });
}



void IA::LocalNodeStore::incSequence(vixac::inout::NodeId nodeId, vixac::inout::NodeId target, vixac::inout::TieType seqName, int64_t inc,  bool additive, IA::DoneFunc f)
{
    
    this->getNode(target, [=](IN::Node * targetNode) {
        if(targetNode->nodeId() != target)
        {
            abort();
        }
    });
    this->getNode(nodeId, [=](IN::Node * base) {
        if(base == NULL)
        {
            abort();
        }
        
      //  std::cout<<"TARGET IS " <<target <<std::endl;
        this->getNode(target, [=](IN::Node * targetNode) {
            if(targetNode->nodeId() != target)
            {
                abort();
            }
            if(targetNode == NULL)
            {
                abort();
            }
            base->incTie(seqName, targetNode, inc, additive);
            f();
        });
    });
}


void IA::LocalNodeStore::subscribeToNode(IN::NodeId nodeId)
{
    abort();
    std::cout<<"TODO LOCAL STORE SUBSCRIBE TO STORE" <<std::endl;
}
