/*User.cpp
User
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "User.h"
#include "Name.h"

#include <iostream>
#include <string>

namespace VI = vixac;
namespace IN = VI::inout;


IN::User::User(IN::NodeStore * store, std::string const& name)
:IN::Phenode(store->newNode(), store)
{
    this->buildPhenode(name);
}

void IN::User::buildPhenode(std::string const& name)
{
    IN::Name theName(store_, name);
    if(this->root()->getPrimary(IN::TieType("name")) != IN::Node::EMPTY_NODE_ID())
    {
        std::cout<<"ERROR WE HAVE A FREAKING NAME ALREADY" <<std::endl;
        abort();
    }
    theName.beTheNameFor(this->root()->nodeId());
}

IN::User::User(IN::Node * root, IN::NodeStore * store)
:IN::Phenode(root, store)
{}

std::string IN::User::name() const
{
    return IN::Name(this->root()->getPrimary(IN::TieType("name")), store_).name();
}
IN::Node * IN::User::nameNode() const
{
    return store_->getNode(this->root()->getPrimary(IN::TieType("name")));
}