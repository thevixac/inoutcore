/*UserSpy.h
UserSpy
 copyright Vixac Ltd. All Rights Reserved 
*/

#ifndef INCLUDED_USERSPY
#define INCLUDED_USERSPY

#include "AsyncFunctor.h"
#include "NodeStoreAsync.h"
#include "PhenodeAsync.h"
#include "AsyncFunctor.h"
#include "UserAsync.h"
#include <string>

namespace vixac
{ 
    namespace ina
    { 
        class UserSpy;
    }
}

class vixac::ina::UserSpy : public vixac::ina::NodeAsyncObserver
{
public:
    UserSpy();
    void spyOnUser(UserAsync const& user);
    void spyOnGroupAdded(NodeIdFunc);
    void spyOnGroupRemoved(NodeIdFunc);
    
    UserAsync * user()  {return &user_;}
private:
    std::string lastName_;

    void tieGained(TieChange t);
    
    NodeIdFunc groupAddedFunc_;
    NodeIdFunc groupRemovedFunc_;
    
    UserAsync user_;
    
    
};
#endif
