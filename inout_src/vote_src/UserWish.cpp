/*UserWish.cpp
UserWish
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "UserWish.h"
#include "Wish.h"
#include "Node.h"
#include <iostream>
#include <string>

namespace VI = vixac;
namespace IA = VI::ina;
namespace IN = vixac::inout;


void IA::UserWishSpy::spyOnUser(IA::UserWish const& user)
{
    user_ = user;
    //    user_.store()->subscribeToNode(user.rootId(), this);
    user_.store()->subscribeToNode(user_.rootId(), this);
}


void IA::UserWishSpy::spyOnWishAdded( bool startup, IA::NodeIdFunc f)
{
    wishAddedFunc_ = f;
    if(startup)
    {
        IA::Wish::getJobs(user_.store(), user_.rootId(), [=](IA::MetaMap map)
        {
            for(auto it = map.begin(), end = map.end(); it != end; ++it)
            {
                IN::NodeId nodeId =it->first;
                f(nodeId);
            }
        });
    }
}

void IA::UserWishSpy::tieGained(IA::TieChange t)
{
    
    if(t.tie.type().type == "is_genie_to")
    {
        wishAddedFunc_(t.tie.target());
    }
}