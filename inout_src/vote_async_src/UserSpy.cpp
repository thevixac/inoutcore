/*UserSpy.cpp
UserSpy
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "UserSpy.h"
#include <iostream>
#include <string>
#include "Node.h"

namespace VI = vixac;
namespace IA = VI::ina;
namespace IN = vixac::inout;



IA::UserSpy::UserSpy()
:groupAddedFunc_([](IN::NodeId){}),
groupRemovedFunc_([](IN::NodeId){})
{

}

void IA::UserSpy::spyOnUser(IA::UserAsync const& user)
{
    user_ = user;
    user_.store()->subscribeToNode(user.rootId(), this);
}

void IA::UserSpy::spyOnGroupAdded(IA::NodeIdFunc f)
{
    groupAddedFunc_ =f;
    user_.getGroups([this](std::vector<IN::NodeId> vec){
        for(int i=0; i < vec.size(); i++)
        {
            groupAddedFunc_(vec[i]);
        }
    });

}

void IA::UserSpy::tieGained(IA::TieChange t)
{
  
    if(t.tie.type().type == "member_of")
    {
        groupAddedFunc_(t.tie.target());
    }
}