/*Vote.h
Vote
 copyright Vixac Ltd. All Rights Reserved 
*/

#ifndef INCLUDED_VOTE
#define INCLUDED_VOTE


#include "Phenode.h"
#include "PhenodeAsync.h"
#include "NodeStore.h"

namespace vixac
{ 
    namespace inout
    { 
        class Vote;
        class User;
    }
    namespace ina
    {
        class UserAsync;
        class VoteAsync;
        
    }
}

class vixac::inout::Vote : public vixac::inout::Phenode
{

public:
 
    Vote(){}
    //breaks the phenode interface rule. This is so that the contructor does not confusingly  take 2 nodeIds as parameters.
    Vote(vixac::inout::NodeStore * store, vixac::inout::User const& user, vixac::inout::NodeId nodeId);
    
    Vote(vixac::inout::NodeId existing, vixac::inout::NodeStore * store);
    Vote(vixac::inout::Node * node, vixac::inout::NodeStore * store);
    
    vixac::inout::Node * voter() const;
    vixac::inout::Node * votee() const;
    
private:

};



class vixac::ina::VoteAsync : public vixac::ina::PhenodeAsync
{
public:
    std::string phenodeType() const{ return "vote";}
    VoteAsync(){}
    VoteAsync(vixac::inout::NodeId nodeId, NodeStoreAsync * store): vixac::ina::PhenodeAsync(nodeId, store){}
    
    VoteAsync(NodeStoreAsync * store,
              vixac::inout::NodeId userRoot,//TODO danger danger, order really matters and its not typesafe
              vixac::inout::NodeId voteeRoot):
    vixac::ina::PhenodeAsync( store),
    userRoot_(userRoot),
    voteeRoot_(voteeRoot)
    {}
    
    void voter(NodeIdFunc);
    void votee(NodeIdFunc);
private:
    void build(NodeIdFunc);
    vixac::inout::NodeId userRoot_;
    vixac::inout::NodeId voteeRoot_;
    
    
};

#endif
