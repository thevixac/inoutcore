/*Phenode.h
Phenode
 copyright Vixac Ltd. All Rights Reserved 
*/

#ifndef INCLUDED_PHENODE
#define INCLUDED_PHENODE

#include "Node.h"
#include "NodeStore.h"
namespace vixac
{ 
    namespace inout
    { 
        class Phenode;
    }
}

/**
 Phenode is any object that can instantiate itself with just a node and a nodestore.
 Rules of Phenode:
  1) must be instantiatable from a root node.
  2) any phenode state outside the store will be gone on object deletion. I don't see why you would ever need phenode state outside the store.
  3) If a phenode can return a NULL node, write it in the membername eg. Node * getNameNullable() const;
  4) Phenodes can (and should) use other phenodes under the hood, but for now the return types of phenode are just Node. Makes for lazy phenode instantiation
  5) (obviously) phenode must garuntee all root nodes and their associated nodes are in a valid state for phenode instantiation. Child Nodes are either nullable in the interface, or they are default constructed.
 */

class vixac::inout::Phenode : public vixac::inout::NodeObserver
{

public:
    virtual bool isValid()const {return isValid_;}
    
    Phenode():/*parentId_(vixac::inout::Node::EMPTY_NODE_ID()),*/ store_(NULL), isValid_(false){}
    Phenode(vixac::inout::Node * topNode, vixac::inout::NodeStore * store);
    
    vixac::inout::Node * root() const {return root_;}
    vixac::inout::NodeStore * store() const {return store_;}
    
    //TODO not sure about this.
    virtual void nodeChanged(vixac::inout::NodeId nodeId);
    virtual void nodeAdded(vixac::inout::NodeId nodeId);
    virtual void childRemovedFrom(vixac::inout::NodeId nodeId);
    
protected:
    bool isValid_;
   // vixac::inout::NodeId parentId_;
    vixac::inout::Node * root_;
    vixac::inout::NodeStore * store_;

};

#endif
