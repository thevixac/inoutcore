/*LocalNodeStore.h
LocalNodeStore
 copyright Vixac Ltd. All Rights Reserved 
*/

#ifndef INCLUDED_LOCALNODESTORE
#define INCLUDED_LOCALNODESTORE

#include "Node.h"
#include "AsyncFunctor.h"
#include "NodeStoreAsync.h"
namespace vixac
{ 
    namespace ina
    { 
        class LocalNodeStore;
    }
}

/**
  A local node store map, but iwth the async interface. mostly for testing.
 */
class vixac::ina::LocalNodeStore : public vixac::ina::NodeStoreAsync
{
public:
    void newNode(vixac::inout::NodeType, NodeIdFunc  f);

    void addTie(vixac::inout::NodeId nodeId, vixac::inout::Tie tie,  DoneFunc  f);
    
    void removeTie(vixac::inout::NodeId nodeId, vixac::inout::Tie tie, DoneFunc f);
    void injectNode(vixac::inout::Node const& n,  DoneFunc  f);
    LocalNodeStore();
    ~LocalNodeStore(){}
    
    void setStringMeta( vixac::inout::NodeId, std::string const& key, std::string const& value, DoneFunc f);
    void setIntMeta(vixac::inout::NodeId, std::string const& key, int64_t value, DoneFunc f);
    void setFloatMeta(vixac::inout::NodeId, std::string const& key, double value, DoneFunc f);
    
    
    void getStringMeta(vixac::inout::NodeId, std::string const& key, StringFunc f);
    void getIntMeta(vixac::inout::NodeId, std::string const& key, IntFunc f);
    void getFloatMeta(vixac::inout::NodeId, std::string const& key, FloatFunc f);
    void getPrimary(vixac::inout::NodeId, vixac::inout::TieType type, NodeIdFunc f);
    void getActiveTies(vixac::inout::NodeId, vixac::inout::TieType type, NodeIdVecFunc f);
    //void getSequence(SequenceRequest, NodeIdVecFunc f);
    
    //TODO wheres nqlmeta at? ffs indeed. Maybe I should write a legit async sql implementation instead of these hacks...

    
    //TODO how am i gna implenet that in here
    void incSequence(vixac::inout::NodeId nodeId, vixac::inout::NodeId target, vixac::inout::TieType seqName, int64_t inc,  bool additive, DoneFunc f);

    
    void subscribeToNode(vixac::inout::NodeId); // does it need a donefunc? dont see why.
    
    
    
private:
     void getNode(vixac::inout::NodeId nodeId,  NodePtrFunc  f);//TODO should be private but i need it for sort
    
//    bool sort(vixac::inout::NodeId lhs, vixac::inout::NodeId rhs);
    
   
    std::map<vixac::inout::NodeId, vixac::inout::Node> nodeMap_;
};
#endif
