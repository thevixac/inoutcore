/*Poll.cpp
Poll
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "Poll.h"
#include "Phelib.h"
#include <iostream>
#include <string>
#include <vector>

namespace VI = vixac;

namespace IN = vixac::inout;
namespace IA = VI::ina;


IN::TieType IA::Poll::fixatedVote()
{
    if(this->rootId() == IN::Node::EMPTY_NODE_ID())
    {
        abort();
    }
    return IA::Phelib::fixate(IN::TieType("vote"), this->rootId());
}

void IA::Poll::build(IA::NodeIdFunc f)
{
    store_->newNode(IN::NodeType("vote_list"),f);
}
void IA::Poll::vote(IN::NodeId user, IN::NodeId votee, IA::DoneFunc f)
{
    this->unvote(user, [=] {
        store_->incSequence(this->rootId(), votee, IN::TieType("votee"), 1, true, [=] {
            store_->addTie(user, IN::Tie(this->fixatedVote(), votee), [=] {
                f();
            });
        });
    });
}

void IA::Poll::unvote(IN::NodeId user, IA::DoneFunc f)
{
    this->personsVote(user, [=](IN::NodeId votee) {
        
        if(votee == IN::Node::EMPTY_NODE_ID())
        {
            f();
        }
        else
        {
            std::cout<<"wehey attempting the unvote" <<std::endl;
            store_->incSequence(this->rootId(), votee, IN::TieType("votee"), -1, true, [=] {
                 store_->removeTie(user, IN::Tie(this->fixatedVote(), votee), f);
            });
        }
    });

}

void IA::Poll::topVote(IA::NodeIdFunc f)
{
    IA::SequenceRequest req(this->rootId(),IN::TieType("votee"));
    req.asc = false;
    req.quantity =1;
    req.sort = IA::SortTypes::Sequence;
    store_->getSequence(req, [=](std::vector<IN::NodeId> vec){
        if(vec.size() ==0)
        {
            abort();
        }
        else if(vec.size() !=1)
        {
            std::cout<<"WARN topVote seq size is not 1, its " <<vec.size()<<std::endl;
        }
        f(vec[0]);
    });
}

void IA::Poll::personsVote(IN::NodeId user, IA::NodeIdFunc f)
{
    store_->getPrimary(user, this->fixatedVote(), f);
}