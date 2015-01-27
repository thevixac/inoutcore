/*Poll.h
Poll
 copyright Vixac Ltd. All Rights Reserved 
*/

#ifndef INCLUDED_POLL
#define INCLUDED_POLL


#include "PhenodeAsync.h"
#include "AsyncFunctor.h"
#include "GraphUtil.h"
#include "Phelib.h"
namespace vixac
{ 
    namespace ina
    { 
        class Poll;
    }
}

//TODO i question the use of this on the client side. it involves full knowledge of what the fuck is going on at all times.
//this is basically undoable. also what happened to subscriptions.

class vixac::ina::Poll : public vixac::ina::PhenodeAsync
{
public:
    Poll(){}
    std::string phenodeType() const{return "poll";};
    Poll(NodeStoreAsync * store): PhenodeAsync(store){}
    Poll(vixac::inout::NodeId existing, NodeStoreAsync * store) : PhenodeAsync(existing, store){}
    
    void vote(vixac::inout::NodeId user, vixac::inout::NodeId votee, DoneFunc);
    void unvote(vixac::inout::NodeId user, DoneFunc);
    
    void topVote(NodeIdFunc);
    void personsVote(vixac::inout::NodeId person, NodeIdFunc);
    
 
    vixac::inout::TieType fixatedVote();
    
private:
    
    void build(NodeIdFunc);
};

#endif
