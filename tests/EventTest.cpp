/*EventTest.cpp
Event
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "Event.h"
#include "NodeStore.h"
#include "VoteList.h" //TODO ffs
#include <gtest/gtest.h>


namespace VI = vixac;
namespace IN = VI::inout;

TEST(Event, aTest)
{
    IN::NodeStore store;
    IN::Event e(&store);
    e.setTitle("abc");
    {
        IN::Node * theTitle = e.getTitleNode();
        ASSERT_FALSE(theTitle == NULL);
        EXPECT_EQ(theTitle->getString("name"), "abc");
        EXPECT_EQ(e.title(), "abc");
    }
    {
        e.setTitle("def");
        IN::Node * theTitle = e.getTitleNode();
        ASSERT_FALSE(theTitle == NULL);
        EXPECT_EQ(e.title(), "def");
    }
}

TEST(Event, whatList)
{
    IN::NodeStore store;
    IN::Event e(&store);
    {
        IN::Node * theWhatListNode = e.getWhatList();
        ASSERT_FALSE(theWhatListNode == NULL);
    }
    {
        IN::Node * topWhat = e.getNullableWhatNode();
        EXPECT_TRUE(topWhat == NULL);
    }
    
    //TODO test non empty vote list here? not sure really, hardly a unit test.
    {
        IN::VoteList whatList(e.getWhatList()->nodeId(), &store);
        EXPECT_EQ(whatList.numVotes(), 0);
    //    EXPECT_EQ(whatList.topVotee->n(), whatList.root()->EMPTY_NODE_ID());
        
        IN::Node * apple = store.newNode();
        apple->setString("type", "apple");
        IN::User alice(&store, "alice");
        
        whatList.vote(alice, apple->nodeId());
        ASSERT_EQ(whatList.numVotes(), 1);
        ASSERT_FALSE(e.getNullableWhatNode() == NULL);
        EXPECT_EQ(e.getNullableWhatNode()->nodeId(), apple->nodeId());
        
    }
}

