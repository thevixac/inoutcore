/*Node.h
Node
 copyright Vixac Ltd. All Rights Reserved 
*/

#ifndef INCLUDED_NODE
#define INCLUDED_NODE

#include "Tie.h"
#include "GraphUtil.h"
#include <vector>
#include <map>
#include <string>

namespace vixac
{ 
    namespace inout
    {
        class Node;
        class NodeIdManager;
        class NodeSubscriber;
        class NodeObserver;
        typedef std::map<TieType, std::vector<Tie> >::const_iterator TieTypeIteratorConst;
        typedef std::map<TieType, std::vector<Tie> >::iterator TieTypeIterator;
    }
}

class vixac::inout::NodeObserver
{
public:
    virtual void nodeGainedTie(vixac::inout::Node * node, vixac::inout::Tie const& tie){}
    virtual void nodeRemovedTie(vixac::inout::Node * node, vixac::inout::Tie const& tie){}
    virtual void nodeRecievedTie(vixac::inout::Node * node, vixac::inout::Tie const& tie, vixac::inout::NodeId sourceNode){}
    virtual void nodeMetaChangedForKey(vixac::inout::Node * node, std::string const& key, std::string const& value){}
    virtual void nodeMetaIntChangedForKey(vixac::inout::Node * node, std::string const& key, int64_t value){}
    virtual void nodeChangedNodeIdFrom(vixac::inout::Node * node, vixac::inout::NodeId nodeId){}
    
};



//TODO this is only used on node tests. do i need it? maybe move it to the test itself or something.
class vixac::inout::NodeIdManager
{
public:
    NodeIdManager():lastId_(0){};
    virtual NodeId nextId() {return ++lastId_;}
    virtual NodeId addNode(vixac::inout::Node const& node) {return this->nextId();}
    
private:
    NodeId lastId_;
};

class vixac::inout::Node
{
public:

    void addObserver(vixac::inout::NodeObserver * observer);
    void removeObserver(vixac::inout::NodeObserver  * observer);
    
    static NodeId EMPTY_NODE_ID() { return NodeId(0);}

    
    //TODO why call these outside the nodestore? Not sure. probably a bad idea. TODO consider locking it down somehow. make the
    //constructors private and friend it to the nodestore or osmething.
    
    Node():nodeId_(EMPTY_NODE_ID()){} //only for stl
    Node(vixac::inout::NodeId nodeId, vixac::inout::INEpoch epoch);
    Node(vixac::inout::NodeIdManager *); //currenlty only used in NodeTest. consider removing this completely
    
    //not sure i want these any more. TODO consider removing them.
    
    vixac::inout::TieTypeIteratorConst getTies(vixac::inout::TieType const& ) const;
    
    
       //VXTODO OPT would be clever if it DIDNT create a new list if nothing has been deleted. would need to return a reference to an existing vector in that case.
    vixac::inout::TieTypeIterator getModifiableTies(vixac::inout::TieType const& );
    vixac::inout::TieTypeIteratorConst tieIteratorEnd() const { return ties_.end();}
 
    
    vixac::inout::NodeId getPrimary(vixac::inout::TieType const&) const;
    int64_t getSeq(TieType, NodeId nodeId);
    std::vector<vixac::inout::Tie> getActiveTies(vixac::inout::TieType const&) const; // only returns nondeleted ties. slower than getTies. depends if delete is a thing.
    
    NodeId nodeId() const {return nodeId_;}
    std::string getString(std::string const& key) const;
    double getFloat(std::string const& key) const;
    int64_t getInt(std::string const& key) const;

    //TODO test this
    vixac::inout::INEpoch createdTime() const {return createdTime_;}
    
    void unobserve();
    //the methods that change nodes persistant state
    void switchNodeIdWithGreatCaution(vixac::inout::NodeId nodeId);


    
    void setString(std::string const& key, std::string const& value);
    

    void setFloat(std::string const& key, double value);
    void setInt(std::string const& key, int64_t value);
    
    //TODO these 3 are a lil silly its true
    std::map<std::string, std::string> allStringMeta() const {return stringData_;}
    std::map<std::string, int64_t> allIntMeta() const { return intData_;}
    std::map<TieType, std::vector<Tie> > allTies() const {return ties_;}
    
    
    bool operator<(vixac::inout::Node const&rhs){return this->nodeId_  < rhs.nodeId();}
    void flagForDeletion();
    void flagTieForDeletion(vixac::inout::Tie const&);
    void deleteTieOutright(vixac::inout::Tie const&);

    
    void incTie(TieType, vixac::inout::Node * target, int64_t value, bool additive = true);
    void addTie(TieType const& type, Node * target);
    void addTie(Tie const&, Node * target);
    void addTieSilentlyWithCaution(Tie const& type, vixac::inout::NodeId nodeId); //since theres
    //no actual target node, there will be no notifications about the added tie, so this is for
    //nodes that arent part of a store.
    
private:
    
    void notifyRecievedTie(vixac::inout::Tie const&, vixac::inout::NodeId sourceId);

    void notifyChangedId(vixac::inout::NodeId oldId);
    std::map<std::string, std::string> stringData_; //TODO VXOPT room for optimisation
    std::map<std::string, int64_t> intData_;
    std::map<std::string, double> floatData_;
    NodeId nodeId_;
    std::map<TieType, std::vector<Tie> > ties_; // this node points to other nodes with these ties.TODO ties should be ordered i guess? maybe theres a separate way to order them or something
    std::vector<vixac::inout::NodeObserver * > observers_; //TODO what happens when the nodeobserver dies? this needs to be removed at that momment.
    vixac::inout::INEpoch createdTime_; //TODO test this

};


#endif
