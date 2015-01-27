/*VoteTest.cpp
Vote
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "Vote.h"
#include "NodeStore.h"
#include "User.h"
#include "LocalNodeStore.h"
#include "UserAsync.h"
#include <gtest/gtest.h>


namespace VI = vixac;
namespace IN = VI::inout;
namespace IA = vixac::ina;

TEST(Vote, aTest)
{
    IN::NodeStore store;
    IN::User alice(&store, "alice");
    IN::Node * apple = store.newNode();
    apple->setString("type", "apple");
    IN::Vote vote(&store, alice, apple->nodeId());
    
    EXPECT_EQ(vote.voter()->nodeId(), alice.root()->nodeId());
    EXPECT_EQ(vote.votee()->nodeId(), apple->nodeId());
    
    IN::Vote copy(store.getNode(vote.root()->nodeId()) ,&store);
    EXPECT_EQ(copy.voter()->nodeId(), alice.root()->nodeId());
    EXPECT_EQ(copy.votee()->nodeId(), apple->nodeId());
    
}

TEST(Vote, asyncTest)
{
    IA::LocalNodeStore store;
    IA::UserAsync alice(&store);
    alice.init([&](){
        store.newNode(IN::NodeType(""),[&](IN::NodeId voteeNodeId){
            store.setStringMeta(voteeNodeId, "type", "apple", [&](){
                IA::VoteAsync vote(&store,alice.rootId(), voteeNodeId );
                vote.init([&](){
                    vote.votee([&](IN::NodeId gotVoteeNodeId){
                        EXPECT_EQ(gotVoteeNodeId, voteeNodeId);
                        vote.voter([&](IN::NodeId gotVoterId){
                            EXPECT_EQ(gotVoterId, alice.rootId());
                            //TODO test copy?
                        });
                    });
                });
            });
        });
    });
}
