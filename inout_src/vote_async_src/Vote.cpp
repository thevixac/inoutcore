/*Vote.cpp
Vote
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "Vote.h"
#include "User.h" 
#include "UserAsync.h"
#include <iostream>
#include <string>

namespace VI = vixac;
namespace IN = VI::inout;
namespace IA = vixac::ina;

IN::Vote::Vote(IN::NodeId existingId, IN::NodeStore * store)
: IN::Phenode(store->getNode(existingId), store)
{}

IN::Vote::Vote(IN::Node * existingNode, IN::NodeStore * store)
: IN::Phenode(existingNode, store)
{}

IN::Vote::Vote(IN::NodeStore * store, IN::User const& user, IN::NodeId votee)
: IN::Phenode(store->newNode(), store)
{
    store_->addTie(this->root()->nodeId(), IN::Tie(IN::TieType("voter"), user.root()->nodeId(), store_->now()));
    store_->addTie(this->root()->nodeId(), IN::Tie(IN::TieType("votee"), votee, store_->now()));
}

IN::Node * IN::Vote::voter() const
{
    return store_->getNode(this->root()->getPrimary(IN::TieType("voter")));
}

IN::Node * IN::Vote::votee() const
{
    return store_->getNode(this->root()->getPrimary(IN::TieType("votee")));
}

void IA::VoteAsync::build(IA::NodeIdFunc f)
{
    store_->newNode(IN::NodeType("vote"),[=](IN::NodeId rootNodeId) {
        store_->addTie(rootNodeId, IN::Tie(IN::TieType("voter"), userRoot_), [=]() { //TODO  VXOPT these can be in parallel, and just return on the second one.
            store_->addTie(rootNodeId, IN::Tie(IN::TieType("votee"), voteeRoot_), [=]() {
                f(rootNodeId);
            });
        });
    });
}

void IA::VoteAsync::votee(IA::NodeIdFunc f)
{
    store_->getPrimary(rootId(), IN::TieType("votee"), f);
}
void IA::VoteAsync::voter(IA::NodeIdFunc f)
{
    store_->getPrimary(rootId(), IN::TieType("voter"), f);
}
