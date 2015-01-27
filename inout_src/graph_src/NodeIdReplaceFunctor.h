/*NodeIdReplaceFunctor.h
NodeIdReplaceFunctor
 copyright Vixac Ltd. All Rights Reserved 
*/

#ifndef INCLUDED_NODEIDREPLACEFUNCTOR
#define INCLUDED_NODEIDREPLACEFUNCTOR


#include "Node.h"
#include "NodeStore.h"

namespace vixac
{ 
    namespace inout
    { 
        class NodeIdReplaceFunctor;
        class NodeIdGenerator;
        class BasicNodeIdGenerator;
        class MasterSlaveNodeIdGenerator;
        class NodeRegistrar;
        class EmptyGenerator;
        class StashGenerator;
        class NodeIdFetch;
        

    }
}

//TODO rename
class vixac::inout::NodeIdReplaceFunctor
{
public:
    NodeIdReplaceFunctor(vixac::inout::NodeStore * store, vixac::inout::NodeId tempNodeId) :
    store_(store), tempNodeId_(tempNodeId){}
    
    void swapTempNodeWithMaster(vixac::inout::NodeId newNodeId);

    
private:
    vixac::inout::NodeStore * store_;
    vixac::inout::NodeId tempNodeId_;
    
};

class vixac::inout::NodeIdGenerator
{
public:
    virtual vixac::inout::NodeId get() =0;
private:
    
};

class vixac::inout::NodeRegistrar
{
public:
    //The registar has the power to wait 5 seconds, then call a store_->getNode, and send the entire bunch to the server, with the freshNodeId request as well.
    //i think thats the clever way to do it. the server can just assume that ALL nodes it recieves are dodgy. the namespacing exists to keep the client state clean, but acutally the server state wil be fine. it will assume that ALL new nodes have temporary Ids, so they will never clash. in the event that ids clash on the client... i guess everything will go tits up.
    //one client can break the server. thats no good.
    virtual void registerThisNode(vixac::inout::NodeIdReplaceFunctor&) =0;
    
    virtual void fetchAvailableNodeIds(vixac::inout::NodeIdFetch&) =0;
    
private:
    
};

class vixac::inout::EmptyGenerator: public vixac::inout::NodeIdGenerator
{
    vixac::inout::NodeId get(){return vixac::inout::Node::EMPTY_NODE_ID();}
};

class vixac::inout::BasicNodeIdGenerator : public vixac::inout::NodeIdGenerator
{
    BasicNodeIdGenerator():last_(0){}
    BasicNodeIdGenerator(vixac::inout::NodeId start=0):last_(start){}
public:
    vixac::inout::NodeId get(){return ++last_;}
private:
    vixac::inout::NodeId last_;
};

class vixac::inout::MasterSlaveNodeIdGenerator
{
public:
    MasterSlaveNodeIdGenerator(vixac::inout::NodeStore * store, vixac::inout::NodeRegistrar * registrar):store_(store), registrar_(registrar){}
    vixac::inout::NodeId get();
private:
    vixac::inout::NodeRegistrar * registrar_;
    vixac::inout::NodeId last_;
    vixac::inout::NodeStore * store_;
    
};

class vixac::inout::StashGenerator : public vixac::inout::NodeIdGenerator
{
public:
    StashGenerator():low_(vixac::inout::Node::EMPTY_NODE_ID()), high_(vixac::inout::Node::EMPTY_NODE_ID()), last_(vixac::inout::Node::EMPTY_NODE_ID()){}
    StashGenerator(vixac::inout::NodeRegistrar * registrar):registrar_(registrar){this->fetchMore();}
    void useThisRange(vixac::inout::NodeId low, vixac::inout::NodeId high){low_ = low; last_ = low; high_ = high; threshHold_ = high_-1;/*low_ + (high_-low_)/2;*/}
    vixac::inout::NodeId get();
private:
    vixac::inout::NodeRegistrar * registrar_;
    void fetchMore();
    vixac::inout::NodeId threshHold_;
    vixac::inout::NodeId low_;
    vixac::inout::NodeId high_;
    vixac::inout::NodeId last_;
    
    
};


//TODO not much point writing functors like this unless registrar simply takes a template function or whatever. anyway. moving on.
class vixac::inout::NodeIdFetch
{
public:
    NodeIdFetch(vixac::inout::StashGenerator * gen):gen_(gen){}
    void operator()(vixac::inout::NodeId low, vixac::inout::NodeId high)
    {
        gen_->useThisRange(low, high);
    }
private:
    vixac::inout::StashGenerator * gen_;
    
};


#endif
