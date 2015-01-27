/*Name.cpp
Name
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "Name.h"
#include <iostream>
#include <string>

namespace VI = vixac;
namespace IN = VI::inout;

IN::Name::Name(IN::NodeStore * store)
:IN::Phenode(store->newNode(), store)
{
    this->setName("");
}

IN::Name::Name(IN::NodeId existingId, IN::NodeStore * store)
: IN::Phenode(store->getNode(existingId), store)
{
    
}
IN::Name::Name(IN::NodeStore * store, std::string const& name)
:IN::Phenode(store->newNode(), store)
{
    this->setName(name);
}

void IN::Name::beTheNameFor(IN::NodeId nodeId)
{
    IN::Node * namedNode = store_->getNode(nodeId);
    
    if(!namedNode)
    {
        std::cout<<"ERROR beTheNamefor has no target node"<<std::endl;
    }
    IN::TieType nameType("name");

    if(namedNode->getPrimary(nameType) != IN::Node::EMPTY_NODE_ID())
    {
        std::cout<<"TODO DOUBLE NAME FAIL" <<std::endl;
        abort();
    }
    store_->addTie(nodeId, IN::Tie(nameType, this->root()->nodeId(), store_->now()));
}
