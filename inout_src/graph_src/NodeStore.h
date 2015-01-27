/*NodeStore.h
NodeStore
 copyright Vixac Ltd. All Rights Reserved 
*/

#ifndef INCLUDED_NODESTORE
#define INCLUDED_NODESTORE

#include "Node.h"
#include "GraphUtil.h"
#include <map>
#include <set>
namespace vixac
{ 
    namespace inout
    { 
        class NodeStore;
        //--//
        class Tie;
        class NodeIdGenerator;
        class BasicNodeIdGenerator;
        class NodeIdReplaceFunctor;
    }
}




/***
 TODO get rid of the nodeIdmanager thing. no need afaik.
 */

class vixac::inout::NodeStore //: public vixac::inout::NodeIdManager
{

public: 

    NodeStore(vixac::inout::NodeIdGenerator * gen){this->useThisGenerator(gen);}
    
    NodeStore():omniObserver_(NULL), generator_(NULL){}
    void useThisGenerator(vixac::inout::NodeIdGenerator * gen){generator_ = gen;}
    void attachObserverToAll(NodeObserver *observer);
    Node * newNode(); //this is the only good way to make a node at the phenode layer.
    Node * getNode(vixac::inout::NodeId nodeId);

    void addTie(NodeId from, Tie tie);
    size_t size() const {return nodes_.size();}
    
    void swapNodeIds(vixac::inout::NodeId, vixac::inout::NodeId);
    vixac::inout::INEpoch now();
    
    //std::map<NodeId, Node> allNodes() const {return nodes_;} //this is obviously not going to fly

    std::map<NodeId, Node > allNodes() const {return nodes_;}
    
    vixac::inout::Node * injectExistingNode(vixac::inout::Node const& node); //reutrns a pointer to the copied node that now lives in this store.
    
    vixac::inout::NodeId highestId() const { return nodes_.rbegin() == nodes_.rend() ? 0 : nodes_.rbegin()->first;}
    
    void moveNode(vixac::inout::Node * node, vixac::inout::NodeId newNodeId ); // the idea being that we no longer need the old id for anything. we just want
    // the new node to be in charge. after this, we no longer garuntee that if you request nodeId x, that the returned node will haave nodeId x.
    //OPTIMISATION here, we have a map of nodeIds to node POINTERS, that way the redirect is just an entry in that table.
    //fuckit lets do it.
    //so nodes are created, then never moved. wait. so we have a vector of nodes. and a map of NodeId to vector location.
private:
    
    vixac::inout::NodeIdGenerator * generator_;
    
    NodeObserver * omniObserver_;

    std::map<NodeId, Node > nodes_; //this could be a set if sets didnt need only use const iterators.
    std::map<NodeId, Node *> nodeLookup_;// can contain many to one mappings to elements in nodes_. so clever!

};




#endif
