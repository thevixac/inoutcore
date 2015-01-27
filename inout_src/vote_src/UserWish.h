/*UserWish.h
UserWish
 copyright Vixac Ltd. All Rights Reserved 
*/

#ifndef INCLUDED_USERWISH
#define INCLUDED_USERWISH



#include "UserAsync.h"
namespace vixac
{ 
    namespace ina
    { 
        class UserWish;
        class UserWishSpy;
        
        // --//
        class NodeStoreAsync;
        class PhenodeAsync;
    }
}

class vixac::ina::UserWish : public vixac::ina::UserAsync
{

public:
    UserWish(){}
    UserWish(NodeStoreAsync * store) : vixac::ina::UserAsync(store){}
    UserWish(vixac::inout::NodeId nodeId, NodeStoreAsync * store):vixac::ina::UserAsync(nodeId, store){}
    

private: 

};

/**
 class vixac::ina::UserSpy : public vixac::ina::NodeAsyncObserver
 {
 public:
 UserSpy();
 //    std::string phenodeType() const{return "userSpy";};
 void spyOnUser(UserAsync const& user);
 void spyOnGroupAdded(NodeIdFunc);
 void spyOnGroupRemoved(NodeIdFunc);
 //    void spyOnGroupChanged(MetaVecFunc);
 
 UserAsync * user()  {return &user_;}
 private:
 std::string lastName_;
 
 void tieGained(TieChange t);
 
 NodeIdFunc groupAddedFunc_;
 NodeIdFunc groupRemovedFunc_;
 
 UserAsync user_;
 
 
 };
 
 */

class vixac::ina::UserWishSpy : public vixac::ina::NodeAsyncObserver
{
public:
    UserWishSpy(){}
    void spyOnUser(UserWish const& user);
    void spyOnWishAdded(bool startup, NodeIdFunc f );
    UserWish * user()  {return &user_;}
    void tieGained(TieChange t);
private:
    UserWish user_;
    NodeIdFunc wishAddedFunc_;
    
    
    
};

#endif
