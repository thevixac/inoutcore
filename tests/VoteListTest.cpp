/*VoteListTest.cpp
VoteList
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "VoteList.h"
#include "Node.h"
#include "User.h"
#include "NodeStore.h"
#include <gtest/gtest.h>


namespace VI = vixac;
namespace IN = VI::inout;

TEST(VoteList, emptyness)
{
    
    IN::NodeStore  store;
    IN::VoteList list(&store);
    ASSERT_EQ(list.numVotes(), 0);
    ASSERT_TRUE(list.topVoteeNullable() == NULL);
    {
        IN::User alice(&store, "alice");
        IN::Node * apple = store.newNode();
        apple->setString("type", "apple");
        EXPECT_EQ(list.numVotes(), 0);
        list.vote(alice, apple->nodeId());
        EXPECT_EQ(list.numVotes(), 1);
        
        ASSERT_FALSE(list.topVoteeNullable() == NULL);
        EXPECT_EQ(list.topVoteeNullable()->nodeId(), apple->nodeId());
        
    }
}

TEST(VoteList, votes)
{
    IN::NodeStore  store;
    IN::VoteList list(&store);
    {
        IN::Node *one = store.newNode();
        one->setString("type", "one");
        IN::Node *two= store.newNode();
        two->setString("type", "two");
        IN::User alice(&store);
        IN::User bob(&store);
        IN::User carol(&store);
        
        IN::NodeId oneId = one->nodeId();
        IN::NodeId twoId = two->nodeId();

        list.vote(alice, oneId);
        ASSERT_FALSE(list.topVoteeNullable() == NULL);
        EXPECT_EQ(list.topVoteeNullable()->nodeId(), oneId);
        EXPECT_EQ(list.topVoteeNullable()->getString("type"), "one");
        
        list.vote(bob, twoId);
        list.vote(carol, twoId);
        EXPECT_EQ(list.topVoteeNullable()->nodeId(), twoId);
        EXPECT_EQ(list.topVoteeNullable()->getString("type"), "two");
        EXPECT_EQ(list.numVotesFor(oneId), 1);
        EXPECT_EQ(list.numVotesFor(twoId), 2);
        EXPECT_EQ(list.numVotesFor( IN::Node::EMPTY_NODE_ID()), 0);

        ASSERT_FALSE(list.getVoteeAtPostion(1)== NULL);
        ASSERT_FALSE(list.getVoteeAtPostion(2)== NULL);
        EXPECT_EQ(list.numVotesFor(list.getVoteeAtPostion(1)->nodeId()), list.numVotesFor(twoId));
        EXPECT_EQ(list.numVotesFor(list.getVoteeAtPostion(2)->nodeId()), list.numVotesFor(oneId));
    }
}

TEST(VoteList, personsVoteNullable)
{
    IN::NodeStore  store;
    IN::VoteList list(&store);
    {
        IN::Node *one = store.newNode();
        IN::Node *two= store.newNode();
        IN::User alice(&store);
        IN::User bob(&store);
        IN::User carol(&store);
        
        IN::NodeId oneId = one->nodeId();
        IN::NodeId twoId = two->nodeId();
        
        list.vote(alice, oneId);
        list.vote(bob, twoId);
        {
            ASSERT_FALSE(list.topVoteeNullable() == NULL);
            ASSERT_FALSE(list.personsVoteNullable(alice) == NULL);
            EXPECT_EQ(list.personsVoteNullable(alice)->nodeId(), oneId);
            
            ASSERT_FALSE(list.getVoteeAtPostion(1)== NULL);
            ASSERT_FALSE(list.getVoteeAtPostion(2)== NULL);
            EXPECT_EQ(list.numVotesFor(list.getVoteeAtPostion(1)->nodeId()), list.numVotesFor(oneId));
            EXPECT_TRUE(list.getVoteeAtPostion(3)== NULL);
            
            EXPECT_EQ((list.numVotes()), 2);
            EXPECT_EQ(list.numVotesFor(oneId), 1);
            EXPECT_EQ(list.numVotesFor(twoId), 1);
        }
        
        list.vote(alice, twoId);
        {
            ASSERT_FALSE(list.personsVoteNullable(alice) == NULL);

            EXPECT_EQ(list.numVotesFor(list.getVoteeAtPostion(1)->nodeId()), list.numVotesFor(twoId));
            EXPECT_TRUE(list.getVoteeAtPostion(2)== NULL);
            EXPECT_EQ(list.personsVoteNullable(alice)->nodeId(), twoId);
            EXPECT_EQ(list.numVotes(), 2);
            EXPECT_EQ(list.numVotesFor(oneId), 0);
            EXPECT_EQ(list.numVotesFor(twoId), 2);
        }

        
    }
}

TEST(VoteList, positions)
{
    IN::NodeStore store;
    IN::VoteList list(&store);
    
    
    IN::User alice(&store, "alice");
    IN::User bob(&store, "bob");
    IN::User carol(&store, "carol");
    IN::User dan(&store, "dan");
    
    IN::Node *  apple = store.newNode();
    apple->setString("name", "apple");
    IN::Node *  banana = store.newNode();
    banana->setString("name", "banana");
    
    {
        list.vote(alice, apple->nodeId()); //apple 1A, banana 0
        EXPECT_FALSE(list.getVoteeAtPostion(1)== NULL);
        EXPECT_TRUE(list.getVoteeAtPostion(2) == NULL);
        EXPECT_EQ(list.getVoteeAtPostion(1)->getString("name"), "apple");
        EXPECT_EQ(list.numVotesFor(apple->nodeId()), 1);
    }
    
    {
        list.vote(bob, banana->nodeId()); // apple 1A, banana 1B
        EXPECT_FALSE(list.getVoteeAtPostion(1)== NULL);
        EXPECT_FALSE(list.getVoteeAtPostion(2) == NULL);
        EXPECT_EQ(list.numVotesFor(apple->nodeId()), 1);
        EXPECT_EQ(list.numVotesFor(banana->nodeId()), 1);
    
    }
    {
        list.vote(carol, banana->nodeId()); // apple 1A, banana 2BC
        EXPECT_EQ(list.getVoteeAtPostion(1)->getString("name"), "banana");
        EXPECT_EQ(list.getVoteeAtPostion(2)->getString("name"), "apple");
        
        EXPECT_EQ(list.numVotesFor(apple->nodeId()), 1);
        EXPECT_EQ(list.numVotesFor(banana->nodeId()), 2);
    
    }
    {
        list.vote(bob, apple->nodeId()); // apple 2AB, banana 1C
        EXPECT_EQ(list.getVoteeAtPostion(1)->getString("name"), "apple");
        EXPECT_EQ(list.getVoteeAtPostion(2)->getString("name"), "banana");
        
        
        EXPECT_EQ(list.numVotesFor(apple->nodeId()), 2);
        EXPECT_EQ(list.numVotesFor(banana->nodeId()), 1);
    }
}

TEST(VoteList, unvote)
{
    IN::NodeStore  store;
    IN::VoteList list(&store);
    {
        IN::Node *one = store.newNode();
        one->setString("type", "one");
        IN::Node *two= store.newNode();
        two->setString("type", "two");
        IN::User alice(&store);
        IN::User bob(&store);
        IN::User carol(&store);
        
        IN::NodeId oneId = one->nodeId();
        IN::NodeId twoId = two->nodeId();
        
        
        list.vote(alice, oneId);
        {
            EXPECT_EQ(list.numVotes(),1);
            EXPECT_EQ(list.numVotesFor(oneId), 1);
            ASSERT_FALSE(list.personsVoteNullable(alice) == NULL);
            EXPECT_EQ(list.personsVoteNullable(alice)->nodeId(), oneId);
        }
        list.unvote(alice);
        {
            EXPECT_EQ(list.numVotes(), 0);
            EXPECT_EQ(list.numVotesFor(oneId), 0);
            EXPECT_TRUE(list.personsVoteNullable(alice) == NULL);
        }
        list.vote(alice, twoId);
        {
            
            EXPECT_EQ(list.numVotes(),1);
            EXPECT_EQ(list.numVotesFor(twoId), 1);
            ASSERT_FALSE(list.personsVoteNullable(alice) == NULL);
            EXPECT_EQ(list.personsVoteNullable(alice)->nodeId(), twoId);
        }
        
        list.unvote(alice);
        {
            EXPECT_EQ(list.numVotes(), 0);
            EXPECT_EQ(list.numVotesFor(oneId), 0);
            EXPECT_TRUE(list.personsVoteNullable(alice) == NULL);
        }
    }
}