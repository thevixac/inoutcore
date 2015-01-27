/*VoteListPhenoSpy.cpp
VoteListPhenoSpy
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "VoteListPhenoSpy.h"

#include "VoteList.h"
#include "Vote.h"
#include "User.h"
#include <iostream>
#include <string>

namespace VI = vixac;
namespace IN = VI::inout;

namespace
{
    class Unsubscribe
    {
        
    public:
        Unsubscribe(IN::NodeObserver * obs):obs_(obs){}
        
        void operator()(IN::Node * node)
        {
            node->removeObserver(obs_);
        }
    private:
        IN::NodeObserver * obs_;
    };
}

IN::VoteListPhenoSpy::VoteListPhenoSpy()
:knownTop_(NULL), numVotesForKnownTop_(0)
{}

void IN::VoteListPhenoSpy::spyOn(IN::VoteList const * list)
{
    this->removeSubscriptions();
    list_= list;
    this->subscribe(list_->root());
    
    votees_.clear();
    
    std::vector<IN::Node *> allVotes = list_->allVotes();
    IN::NodeStore * store = list_->store();
    for(int i=0;i < allVotes.size();i++)
    {
        votees_.insert(IN::Vote(allVotes[i], store).votee()->nodeId());
    }
}

void IN::VoteListPhenoSpy::removeSubscriptions()
{
    std::for_each(subscriptions_.begin(), subscriptions_.end(), ::Unsubscribe(this));
}


void IN::VoteListPhenoSpy::subscribe(IN::Node * node)
{
    subscriptions_.push_back(node);
    node->addObserver(this);
    

    
}

void IN::VoteListPhenoSpy::checkForNewTop()
{
    IN::Node * top = list_->getVoteeAtPostion(1);
    if(top == NULL)
    {
        return;
    }
    size_t numVotes = list_->numVotesFor(top->nodeId());
    size_t numVotesForSecond =0;
    IN::Node * secondPlaceVotee = list_->getVoteeAtPostion(2);
    if(secondPlaceVotee != NULL)
    {
        numVotesForSecond = list_->numVotesFor(secondPlaceVotee->nodeId());
    }
    if(numVotes != numVotesForSecond && knownTop_ != top)
    {
        knownTop_ = top;
        this->spiedTopChangedTo(top);
    }
}


void IN::VoteListPhenoSpy::nodeGainedTie(vixac::inout::Node * node, vixac::inout::Tie const& tie)
{
    if(node->nodeId() == list_->root()->nodeId() && tie.type().type == std::string("vote"))
    {
        IN::Vote newlyAddedVote(tie.target(), list_->store());
        
        IN::NodeId voteeNodeId = newlyAddedVote.votee()->nodeId();
        if(votees_.find(voteeNodeId)== votees_.end())
        {
            votees_.insert(voteeNodeId);
            this->spiedNewVoteeAdded(newlyAddedVote);
        }
        this->spiedVoteAdded(newlyAddedVote);
        this->checkForNewTop();
        this->subscribe(list_->store()->getNode(tie.target()));
    }
    else // its a vote
    {
        
    }
}

void IN::VoteListPhenoSpy::nodeRemovedTie(vixac::inout::Node * node, vixac::inout::Tie const& tie)
{
    if(node->nodeId() == list_->root()->nodeId() && tie.type().type == std::string("vote"))
    {
        IN::Vote removedVote(tie.target(), list_->store());
        this->spiedVoteRemoved(removedVote);
        this->checkForNewTop();
    }
    else // its a vote
    {
        
    }
}

void IN::VoteListPhenoSpy::nodeChangedNodeIdFrom(vixac::inout::Node * node, vixac::inout::NodeId nodeId)
{
    IN::Vote removedVote(node->nodeId(), list_->store());
    this->spiedVoteRemoved(removedVote);
  
    //TODO we dont know if a vote was addded or not at this point. User might have just UNVOTED. so we need to track if one was added
    
    IN::User user(removedVote.voter(), list_->store());
    IN::Node * votee = list_-> personsVoteNullable(user);
    
    IN::Vote addedVote(list_->store(), user, votee->nodeId());
    this->spiedVoteAdded(addedVote);
    this->checkForNewTop();
}

void IN::VoteListPhenoSpy::nodeRecievedTie(vixac::inout::Node * node, vixac::inout::Tie const& tie, vixac::inout::NodeId sourceNode)
{
    
}

void IN::VoteListPhenoSpy::nodeMetaChangedForKey(vixac::inout::Node * node, std::string const& key, std::string const& value)
{
    
}

IN::VoteListPhenoSpy::~VoteListPhenoSpy()
{
    this->removeSubscriptions();
}
