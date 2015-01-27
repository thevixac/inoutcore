/*VoteList.cpp
VoteList
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "VoteList.h"
#include "Vote.h"
#include "User.h"
#include "NodeStoreAsync.h"

#include <iostream>
#include <string>

namespace VI = vixac;
namespace IN = VI::inout;
namespace IA = vixac::ina;

IN::VoteList::VoteList(IN::NodeStore * store)
:IN::Phenode(store->newNode(), store)
{}

IN::VoteList::VoteList(IN::NodeId existingId, IN::NodeStore * store)
:IN::Phenode(store->getNode(existingId), store)
{
    
}

IN::Node * IN::VoteList::topVoteeNullable() const
{
    /***
     
     */
    std::multimap<size_t,IN::Node *> list = this->sortedVoteeList();
    return list.size() > 0 ? list.rbegin()->second : NULL;
}
IN::Node * IN::VoteList::getVoteeAtPostion(int place) const
{
    std::multimap<size_t,IN::Node *> list = this->sortedVoteeList();

    if(list.size() < place)
    {
        return NULL;
    }
    int count =0;
    for(std::multimap<size_t,IN::Node *>::const_reverse_iterator it = list.rbegin(), end = list.rend(); it !=end; ++it)
    {
        count++;
        if(count == place)
        {
            return it->second;
        }
    }
    abort();
    return NULL; //should never happen
}
size_t IN::VoteList::numVotes() const
{
    return this->allVotes().size();
}

//VXTODO OPT return nodeIds instead, can be fetched by client if needed
std::vector<IN::Node *> IN::VoteList::allVotes() const
{
    std::vector<IN::Node *> allVotes;
    std::vector<IN::Tie> ties = root()->getActiveTies(IN::TieType("vote"));
    for(int i=0;i < ties.size(); i++)
    {
        allVotes.push_back(store_->getNode(ties[i].target()));
    }

    return allVotes;

}
std::multimap<size_t,IN::Node *> IN::VoteList::sortedVoteeList() const
{
    std::vector<IN::Node *> allVotes = this->allVotes();
    
    std::map<IN::Node *, size_t> voteeMap;
    for(int i=0; i < allVotes.size(); i++)
    {
        IN::Vote vote(allVotes[i], store_);
        if(vote.votee() == NULL)
        {
            std::cout<<"whattf" << i <<std::endl;
        }
        std::map<IN::Node *, size_t>::iterator it = voteeMap.find(vote.votee());
        if(it == voteeMap.end())
        {
            voteeMap.insert(std::make_pair(vote.votee(), 1));
        }
        else
        {
            voteeMap[vote.votee()] = voteeMap[vote.votee()]+1;
        }
    }
    std::multimap<size_t,IN::Node *> result;
    for(std::map<IN::Node *, size_t>::const_iterator it = voteeMap.begin()
        , end = voteeMap.end(); it != end; ++it)
    {
        result.insert(std::make_pair(it->second, it->first));
    }
    return result;
}
size_t IN::VoteList::numVotesFor(IN::NodeId nodeId) const
{
    std::vector<IN::Node *> allVotes = this->allVotes();
    size_t count(0);
    for(int i=0; i< allVotes.size();i ++)
    {
        IN::Vote vote(allVotes[i], store_);
        if(vote.votee()->nodeId() == nodeId)
        {
            count++;
        }
    }
    return count;
}

IN::Node * IN::VoteList::personsVoteNullable(IN::User const& user) const
{
     std::vector<IN::Node *> allVotes = this->allVotes();
    
    for(int i=0; i< allVotes.size();i ++)
    {
        IN::Vote vote(allVotes[i], store_);
        if(vote.voter()->nodeId() == user.root()->nodeId())
        {
            return vote.votee();
        }
    }
    return NULL;
}

void IN::VoteList::vote(IN::User const& user, IN::NodeId nodeId)
{
    std::vector<IN::Node *> allVotes = this->allVotes();
    
    for(int i=0; i< allVotes.size();i ++)
    {
        IN::Vote oldVote(allVotes[i], store_);
        if(oldVote.voter()->nodeId() == user.root()->nodeId())
        {
            this->unvote(user);
        }
    }
    IN::Vote vote(store_, user, nodeId);
    store_->addTie(this->root()->nodeId(), IN::Tie(IN::TieType("vote"), vote.root()->nodeId(), store_->now()));
}

void IN::VoteList::unvote(IN::User const& user)
{
    std::vector<IN::Tie> ties = root()->getActiveTies(IN::TieType("vote"));
    
    IN::NodeId userNodeId = user.root()->nodeId();
    for(int i=0;i < ties.size(); i++)
    {
        IN::Vote vote = IN::Vote(store_->getNode(ties[i].target()), store_);
        if(vote.voter()->nodeId() == userNodeId)
        {
            root()->flagTieForDeletion(ties[i]);
            return;
        }
    }
}

/////async

/**
 void vote(vixac::inout::NodeId user, vixac::inout::NodeId votee, DoneFunc);
 void unvote(vixac::inout::NodeId user, DoneFunc);
 
 void topVote(NodeIdFunc);
 void allVotes(NodeIdVecFunc);
 void personsVote(vixac::inout::NodeId person);
 
 
 private:
 void build(NodeIdFunc);
*/


