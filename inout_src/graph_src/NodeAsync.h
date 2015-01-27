/*NodeAsync.h
NodeAsync
 copyright Vixac Ltd. All Rights Reserved 
*/

#ifndef INCLUDED_NODEASYNC
#define INCLUDED_NODEASYNC


#include "Node.h"
#include "NodeStoreAsync.h"

#include "AsyncFunctor.h"
namespace vixac
{ 
    namespace ina
    { 
        class NodeAsync;
    }
}


/**
  So if the store isnt local, you'll need a node handle rather than an actual node. this is what that is. Maybe you should just pawn off your work to the freaking store.
 
 */
class vixac::ina::NodeAsync
{

public:
        
    NodeAsync():nodeId_(vixac::inout::Node::EMPTY_NODE_ID()){}
    NodeAsync(vixac::ina::NodeStoreAsync * store, vixac::inout::NodeId nodeId):nodeId_(nodeId), store_(store){}
    bool operator<(vixac::ina::NodeAsync const&rhs){return this->nodeId_  < rhs.nodeId();}
    vixac::inout::NodeId nodeId() const {return nodeId_;}
    
    
    void addTie(vixac::inout::TieType type, vixac::inout::NodeId targetId,  DoneFunc f, bool notifyTarget=true);
    void setStringMeta(std::string const& key, std::string const& value, DoneFunc f);
    void setIntMeta(std::string const& key, int64_t value, DoneFunc f);
    void setFloatMeta(std::string const& key, double value, DoneFunc f);
    
    
    void getStringMeta(std::string const& key, StringFunc f);
    void getFloatMeta(std::string const& key, FloatFunc f);
    void getIntMeta(std::string const& key, IntFunc f);
    void getPrimary(vixac::inout::TieType type, NodeIdFunc f);
    void getActiveTies(vixac::inout::TieType type, NodeIdVecFunc f);
    

private:
    vixac::ina::NodeStoreAsync * store_;
    vixac::inout::NodeId nodeId_;

};

#endif
