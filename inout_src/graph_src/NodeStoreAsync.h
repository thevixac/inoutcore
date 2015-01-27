/*NodeStoreAsync.h
NodeStoreAsync
 copyright Vixac Ltd. All Rights Reserved 
*/

#ifndef INCLUDED_NODESTOREASYNC
#define INCLUDED_NODESTOREASYNC


#include "Node.h"
#include <iostream>
#include "AsyncFunctor.h"
#include <set>
namespace vixac
{ 
    namespace ina
    {
        class NodeGenAsync;
        // --//
        class NodeStoreAsync;
        class NodeHandle;
        
        class PhenodeAsync;
 
        class NodeAsyncObserver;
        
        
        namespace SortTypes//TODO server needs to understand this enum
        {
            enum Type
            {

                Update =0,
                Creation=1,
                Sequence=2,

            };
        }
 
        const int64_t QUANTITY_ALL =0;
        
        struct SequenceRequest {

            SequenceRequest():
            isSet_(false),
            nodeId(0),
            type(""),
            quantity(QUANTITY_ALL), // that means all
            asc(true),
            skip(0),
            sort(SortTypes::Creation)
            {}
            SequenceRequest(vixac::inout::NodeId nodeId, vixac::inout::TieType type):
            isSet_(true),
            nodeId(nodeId),
            type(type),
            quantity(QUANTITY_ALL), // that means all
            asc(true),
            skip(0),
            sort(SortTypes::Creation)
            {}
            
            vixac::inout::NodeId nodeId;
            vixac::inout::TieType type;
            int64_t quantity;
            bool asc;
            int64_t skip;
            SortTypes::Type sort;
            bool isSet_; 
            
        };


    }
}


class vixac::ina::NodeAsyncObserver
{
public:
    
    NodeAsyncObserver():store_(NULL){}
    void bindToStore(vixac::ina::NodeStoreAsync * store) {
        store_= store;
        
    }
    virtual void tieGained(TieChange  t){std::cout<<"default tie gained called " <<t.tie.type().type <<std::endl;}
    virtual void tieRemoved(TieChange  t){}
    virtual void tieRecieved(TieChange  t){}
    virtual void stringChanged(StringChange t){}
    virtual void intChanged(IntChange  t){}
    virtual void nodeIDChanged(NodeIdChange t){}
    
    ~NodeAsyncObserver();
    
    
private:
    vixac::ina::NodeStoreAsync * store_; //TODO make weak ptr
};


class vixac::ina::NodeStoreAsync
{

public:
    friend class PhenodeAsync;
    friend class NodeAsync;

    void useThisGenerator(vixac::ina::NodeGenAsync * gen){gen_ = gen;}

    virtual void newNode(vixac::inout::NodeType,  NodeIdFunc  f){f(-1);}
    
    //i just want to do getNode really. thats the trick. so it has a const state, or soemthing?
    // ok so the store is used to collect that data. i guess so it needs to be possible to call all that shit.
    //which then gets wrapped in the construction of the node. but th enode needs to be able to get to its biznis.
    //the async store  hmm.
    //ok what if getNode reutrned somethng eles. i dont know what. something that knows how to populateitself. thats what i was
    virtual void addTie(vixac::inout::NodeId, vixac::inout::Tie, DoneFunc  f){}
    virtual void removeTie(vixac::inout::NodeId, vixac::inout::Tie, DoneFunc f){}
    virtual void injectNode(vixac::inout::Node const& n, DoneFunc  f){}
    
    
    
    //TODO consider a variant. consider moving over to Meta.
    virtual void setStringMeta( vixac::inout::NodeId, std::string const& key, std::string const& value, DoneFunc f){}
    virtual void setIntMeta(vixac::inout::NodeId, std::string const& key, int64_t value, DoneFunc f){}
    virtual void setFloatMeta(vixac::inout::NodeId, std::string const& key, double value, DoneFunc f){}
    virtual void getStringMeta(vixac::inout::NodeId, std::string const& key, StringFunc f){}
    virtual void getFloatMeta(vixac::inout::NodeId, std::string const& key, FloatFunc f){}
    virtual void getIntMeta(vixac::inout::NodeId, std::string const& key, IntFunc f){}
    
    //ok if i write here get getMeta, and getMetaVec. then i can use a string to request phenetic crap.
    virtual void getPrimary(vixac::inout::NodeId, vixac::inout::TieType type, NodeIdFunc f){}
    virtual void getActiveTies(vixac::inout::NodeId, vixac::inout::TieType type, NodeIdVecFunc f){}//takes a node and fin

   //virtual void getBulkMeta(NodeIdVec const& vec, std::vector<std::string> const& fieldVec, MetaVecFunc f){}
    virtual void phenGetMeta(const PhenodeAsync *   phen, std::string const& phenCall, KeyMetaMap const& data, MetaVecFunc f){}
    virtual void phenGetTies(const PhenodeAsync *   phen,  std::string const& phenCall, KeyMetaMap const& data, NodeIdVecFunc f){}
    virtual void phenDo(const PhenodeAsync * phen, std::string const& phenCall, KeyMetaMap const& data, BoolFunc f){}
    
   //the sequence request is used for the fields of asc, quantity, sort, and skip.
    //i could change the api to be a vector of sequenceReqeusts. apart frmo the node field, that would make a whole lot more sense. THat way i expose
    //sorting and whatnot down the request, which is really cool, and i basically have that for free the way ive implemented it in eve.
    
    virtual void getNqlMeta(NodeIdVec const& vec, std::vector<vixac::inout::TieType> const& walkVec, std::vector<std::string> const& fieldVec, SequenceRequest, MetaVecFunc f){}
    
    //TODO ad these to local node store? I guess. Also why use ties if i am goign to use sequences.
    // its just ties, but a different table.
  //  virtual void getSequence(vixac::inout::NodeId nodeId,   vixac::inout::TieType seqName,int64_t quantity, bool asc, int64_t skip, NodeIdVecFunc f){}
    
    //TODO what tod o with getsequence?
    virtual void getSequence(SequenceRequest, NodeIdVecFunc f){}
    virtual void incSequence(vixac::inout::NodeId nodeId, vixac::inout::NodeId target, vixac::inout::TieType seqName, int64_t inc,  bool additive, DoneFunc f){}

    
    

    void subscribeToNode(vixac::inout::NodeId, NodeAsyncObserver * obs );
    void unsubToNode(vixac::inout::NodeId, NodeAsyncObserver * obs );
    void unsub(NodeAsyncObserver * obs);
    
    
    //TODO this is a fat interface. when this gets called , the obsevers are notified. This is the way that a proxy store NOTIFIIES this async store.
    // its a shame thta this is puiblic. I'd like to have it as its own class or something.
    // so you can say  postman = store->postman;
    //then i can say serverconnection.usethispostman(postman).
    //postman is a friend to nodestoreasync, and can privately call tieGained and whatnot.
    //very clever. thanks.
    void tieGained(TieChange t);
    void tieRemoved(TieChange t);
    void stringChanged(StringChange  t);
    void intChanged(IntChange t);
    void nodeIdChanged(NodeIdChange t);
    
    
    //used by the phenode Api, the client Id is the person who is making the calls. This is how eventually the api will return different results based on whos asking.
    vixac::inout::NodeId clientId() const {return clientId_;}
    void setClientId(vixac::inout::NodeId nodeId) {clientId_ = nodeId;}
protected:
    //implement these if you need to some extra work to ensure that the subscription actually does anything.
    virtual void handleSubscriptionToNode(vixac::inout::NodeId, NodeAsyncObserver * obs){};
    virtual void handleUnsubToNode(vixac::inout::NodeId, NodeAsyncObserver * obs){};

    
    //TODO make private? Its only protected for a gtest
    std::map<vixac::inout::NodeId, std::set<NodeAsyncObserver *> > observers_;

    
private:
    vixac::inout::NodeId clientId_;

    void tieRecieved(TieChange t);
    std::set<NodeAsyncObserver *> observersForNodeId(vixac::inout::NodeId nodeId);
    

    vixac::ina::NodeGenAsync * gen_;

};








#endif
