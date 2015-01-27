/*VoteList.h
VoteList
 copyright Vixac Ltd. All Rights Reserved 
*/

#ifndef INCLUDED_VOTELIST
#define INCLUDED_VOTELIST



#include "Node.h"
#include "Phenode.h"
#include "PhenodeAsync.h"
#include <vector>
#include <map>
namespace vixac
{ 
    namespace inout
    {
        class VoteList;
        typedef bool ChangeState; // the idea is maybe things like "new top, etc. maybe instead of this we have subscribers
        
        
        //forward
        class User;
        
    }
    
    namespace ina
    {
        class VoteListAsync;
        class NodeStoreAsync;
    }
}

// what would the subscriber look like.
// firstly it would need to subscribe to the other bits,
//secondly, it would expose the following
//userchangedvoteto x
// etc. 
class vixac::inout::VoteList : public vixac::inout::Phenode
{

public:

    VoteList(){}//ffs
    VoteList(vixac::inout::NodeId existing, vixac::inout::NodeStore *);
    VoteList(vixac::inout::NodeStore * store);
    void vote(vixac::inout::User const& user, vixac::inout::NodeId);
    
    
    void unvote(vixac::inout::User const& user);
    
    
    //TODO the topvote selection is undefined on a tie. Really I dont know what to do here.
    vixac::inout::Node * topVoteeNullable() const;
    
    //TODO do we need this?
    vixac::inout::Node * getVoteeAtPostion(int place) const;
    
    size_t numVotes() const;
    size_t numVotesFor(vixac::inout::NodeId nodeId) const;
    std::vector<vixac::inout::Node *> allVotes() const; //each is a Vote object.
    std::multimap<size_t,vixac::inout::Node *> sortedVoteeList() const; // front is the mosted voted for item, down to the last.
    
    vixac::inout::Node * personsVoteNullable(vixac::inout::User const& user) const;
    
    size_t numVotables() const;//TODO implement?
    
    std::string listType() const { return root()->getString("vote_list_type");}
    void setListType(std::string const& str) const { root()->setString("vote_list_type", str);}
private:

};



#endif
