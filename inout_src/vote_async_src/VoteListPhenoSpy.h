/*VoteListPhenoSpy.h
VoteListPhenoSpy
 copyright Vixac Ltd. All Rights Reserved 
*/

#ifndef INCLUDED_VOTELISTPHENOSPY
#define INCLUDED_VOTELISTPHENOSPY



#include "VoteList.h"
#include "Node.h"

#include <vector>
#include <map>
#include <set>
namespace vixac
{ 
    namespace inout
    { 
        class VoteListPhenoSpy;
        
        class Vote;
        class Tie;
        
    }
}

class vixac::inout::VoteListPhenoSpy : public vixac::inout::NodeObserver
{

public:
    
    VoteListPhenoSpy();//
    void spyOn(vixac::inout::VoteList const* list);
    

    //override these to get your callbacks when cool shit happens.
    virtual void spiedNewVoteeAdded(vixac::inout::Vote const& vote){}
   // virtual void spiedOldvoteeRemoved(vixac::inout::Vote const& vote){} //TODO do these fuckers block the call to spiedvoteadded? i gues not, its separate ainti .

    virtual void spiedVoteRemoved(vixac::inout::Vote const& vote){}
    virtual void spiedVoteAdded(vixac::inout::Vote const& vote){}
    virtual void spiedTopChangedTo(vixac::inout::Node * ){}
    
    ~VoteListPhenoSpy();
    
    /// NodeObserver
    void nodeGainedTie(vixac::inout::Node * node, vixac::inout::Tie const& tie);
    void nodeRemovedTie(vixac::inout::Node * node, vixac::inout::Tie const& tie);
    void nodeRecievedTie(vixac::inout::Node * node, vixac::inout::Tie const& tie, vixac::inout::NodeId sourceNode);
    void nodeMetaChangedForKey(vixac::inout::Node * node, std::string const& key, std::string const& value);
    void nodeChangedNodeIdFrom(vixac::inout::Node * node, vixac::inout::NodeId nodeId);
    
    
    int64_t numVotees() const { return votees_.size();}
    int64_t numVoters() const {return 0;} //TODO implement if its handy
    //todo getters on this?
private:
    void checkForNewTop();
    void subscribe(vixac::inout::Node *);
    
    vixac::inout::VoteList  const *list_;
    vixac::inout::Node * knownTop_;
    size_t numVotesForKnownTop_;
    
    void removeSubscriptions();
    void handleRootNodeUpdate();
    void handleNewNode();
    std::vector<vixac::inout::Node *> subscriptions_;
    
    std::set<vixac::inout::NodeId> votees_;
//    std::map<vixac::inout::NodeId, vixac::inout::Node *> knownVotes_;
    

    

};

#endif
