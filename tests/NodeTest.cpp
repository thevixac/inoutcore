/*NodeTest.cpp
Node
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "Node.h"

#include <gtest/gtest.h>
#include <vector>

namespace VI = vixac;
namespace IN = VI::inout;

namespace
{
    struct SimpleObserver : public IN::NodeObserver
    {
    public:
        SimpleObserver()
         :
        nodeGainedCalls_(0),
        nodeRecievedCalls_(0),
        nodeMetaChangedCalls_(0),
        nodeChangedNodeIdCalls_(0)
        {}

        void nodeGainedTie(IN::Node * node, IN::Tie const& tie)
        {
            lastGainedTie_ = tie;
            nodeGainedCalls_++;
        }
        
        void nodeRecievedTie(IN::Node * node, IN::Tie const& tie, IN::NodeId sourceNode)
        {
            lastRecievedTie_ = tie;
            lastRecievedSourceNodeId_ = sourceNode;
            
            nodeRecievedCalls_++;
        }
        
        void nodeMetaChangedForKey(IN::Node * node, std::string const& key, std::string const& value)
        {
            lastKey_ = key;
            lastValue_= value;
            nodeMetaChangedCalls_++;
        }
        
        void nodeChangedNodeIdFrom(IN::Node * node, IN::NodeId nodeId)
        {
            lastOldNodeId_ = nodeId;
            nodeChangedNodeIdCalls_++;
        }
        
        IN::Tie lastGainedTie_;
        IN::Tie lastRecievedTie_;
        std::string lastKey_;
        std::string lastValue_;
        IN::NodeId lastOldNodeId_;
        IN::NodeId lastRecievedSourceNodeId_;
        int nodeGainedCalls_;
        int nodeRecievedCalls_;
        int nodeMetaChangedCalls_;
        int nodeChangedNodeIdCalls_;
        
    };
}

TEST(Node, aTest)
{
    
    IN::NodeIdManager g;
    IN::Node node(&g);
    IN::Node node2(&g);
    IN::Node node3(&g);
    IN::TieType SOME_TIE_TYPE("42");
    node.addTie(SOME_TIE_TYPE, &node2);
    node.addTie(SOME_TIE_TYPE, &node3);
    
    IN::TieTypeIteratorConst tieIt = node.getTies(SOME_TIE_TYPE);
    ASSERT_FALSE(tieIt == node.tieIteratorEnd());
    EXPECT_EQ(tieIt->second.size(), 2);
}

TEST(Node, observerations)
{
    IN::NodeIdManager g;
    IN::Node alice(&g);
    IN::Node bob(&g);
    
    ::SimpleObserver aliceObserver;
    alice.addObserver(&aliceObserver);

    {
        alice.addTie(IN::TieType("friend"), &bob);
        EXPECT_EQ(aliceObserver.nodeGainedCalls_, 1);
        EXPECT_EQ(aliceObserver.lastGainedTie_.target(), bob.nodeId());
        EXPECT_EQ(aliceObserver.lastGainedTie_.type().type, "friend" );
    }
    {
        bob.addTie(IN::TieType("secret_crush"), &alice);
        EXPECT_EQ(aliceObserver.nodeRecievedCalls_, 1);
        EXPECT_EQ(aliceObserver.lastRecievedTie_.target(), alice.nodeId());
        EXPECT_EQ(aliceObserver.lastRecievedTie_.type().type, "secret_crush" );
    }
    {
        alice.setString("name", "alice");
        EXPECT_EQ(aliceObserver.lastKey_, "name");
        EXPECT_EQ(aliceObserver.lastValue_, "alice");
        alice.setString("name", "alicia");
        EXPECT_EQ(aliceObserver.lastValue_, "alicia");
        EXPECT_EQ(aliceObserver.nodeMetaChangedCalls_, 2);
    }
    {
        IN::NodeId oldAliceId = alice.nodeId();
        alice.switchNodeIdWithGreatCaution(bob.nodeId());
        bob.switchNodeIdWithGreatCaution(oldAliceId);
        EXPECT_EQ(aliceObserver.lastOldNodeId_, oldAliceId);
        EXPECT_EQ(aliceObserver.nodeChangedNodeIdCalls_, 1);
        alice.setString("name", "alison");
        EXPECT_EQ(aliceObserver.nodeMetaChangedCalls_, 3);
    }
    {
        EXPECT_EQ(aliceObserver.nodeGainedCalls_, 1);
        EXPECT_EQ(aliceObserver.nodeRecievedCalls_, 1);
        EXPECT_EQ(aliceObserver.nodeMetaChangedCalls_, 3);
    }
    
}

TEST(Node, nodeCopy)
{
    IN::NodeIdManager g;
    IN::Node alice(&g);
    IN::Node bob(&g);
    IN::Node carol(&g);
    alice.setString("name", "alice");
    alice.setInt("age", 42);
    
    {
        IN::Tie f = IN::Tie(IN::TieType("friend"), bob.nodeId());
        alice.addTie(f, &bob);
    }
    {
        IN::Tie f = IN::Tie(IN::TieType("friend"), carol.nodeId());
        alice.addTie(f, &bob);
        alice.flagTieForDeletion(f);
    }
    
    IN::Node aliceClone = alice; //straight up copy
    
    //TODO what about the observers? probably need to make sure i return a blank bunch of observers, or i always wipe observers
    
    EXPECT_EQ(alice.nodeId(), aliceClone.nodeId());
    EXPECT_EQ(alice.getString("name"), aliceClone.getString("name"));
    EXPECT_EQ(alice.getInt("age"), aliceClone.getInt("age"));

    std::vector<IN::Tie> ties = alice.getActiveTies(IN::TieType("friend"));
    std::vector<IN::Tie> cloneTies = aliceClone.getActiveTies(IN::TieType("friend"));
    ASSERT_EQ(ties.size(), cloneTies.size());
    EXPECT_EQ(ties.size(), 1);
    for(int i=0;i < ties.size(); i++)
    {
        EXPECT_EQ(ties[i].type().type, cloneTies[i].type().type);
        EXPECT_EQ(ties[i].target(), cloneTies[i].target());
    }
    

        
}
/*
TEST(Node, parenting)
{
    IN::NodeIdManager g;
    IN::Node car(&g);
    IN::Node alice(&g);
    IN::Node bob(&g);
    IN::Node carol(&g);
    IN::Node fuselage(&g, &car);
    IN::Node driverSeat(&g, &fuselage);
    IN::Node passengerSeat(&g, &fuselage);
    
    const IN::TieType drives("drives");
    const IN::TieType passenger("passenger");
    const IN::TieType sitsOn("sitsOn");
    alice.addTie(drives, &car);
    alice.addTie(sitsOn, &driverSeat);
    bob.addTie(passenger, &car);
    bob.addTie(sitsOn, &passengerSeat);
    {
        EXPECT_TRUE(car.isRoot());
        EXPECT_TRUE(alice.isRoot());
        EXPECT_TRUE(bob.isRoot());
        EXPECT_FALSE(fuselage.isRoot());
        EXPECT_EQ(car.nodeId(), fuselage.parent()->nodeId());
    }
    {
        std::vector<IN::Node *> carChildren = car.children();
        ASSERT_EQ(carChildren.size(), 1);
        EXPECT_EQ(carChildren[0]->nodeId(), fuselage.nodeId());
    }
    {
        IN::TieTypeIteratorConst it = alice.getTies(drives);
        EXPECT_FALSE(it == alice.tieIteratorEnd());
        ASSERT_EQ(it->second.size(), 1);
        EXPECT_EQ(it->second[0].target(), car.nodeId());
    }
    {
        IN::TieTypeIteratorConst  it = car.getTieEnds(drives);
        ASSERT_FALSE(it == car.tieEndsIteratorEnd());
        ASSERT_EQ(it->second.size(), 1);
        ASSERT_EQ(it->second[0].target(), alice.nodeId());
    }    
}
 

TEST(Node, observers)
{

    //TODO this test is old now, as inherited observation is dead
    IN::NodeIdManager g;
    IN::Node bike(&g);
    IN::Node wheel(&g);
    IN::Node spoke(&g);;
    IN::Node spokeNipple(&g);
    IN::Node frame(&g);
    
    ::SimpleObserver bikeObserver;
    bike.addObserver(&bikeObserver);
    
    ::SimpleObserver wheelObserver;
    wheel.addObserver(&wheelObserver);
    
    ::SimpleObserver spokeNippleObserver;
    spokeNipple.addObserver(&spokeNippleObserver);
    
    EXPECT_EQ(bikeObserver.lastAdded_, 0);
    IN::Node seat(&g);
    ::SimpleObserver seatObserver;
    seat.addObserver(&seatObserver);
    
    //observation is no longer inherited, so this test is dead.
    //EXPECT_EQ(bikeObserver.lastAdded_, seat.nodeId());
    
}
*/

TEST(Node, metaString)
{
    IN::NodeIdManager g;
    IN::Node node(&g);
    {
        node.setString("key", "value");
        EXPECT_EQ(node.getString("key"), "value");
    }
    {
        
        node.setString("key", "nextValue");
        EXPECT_EQ(node.getString("key"), "nextValue");
    }
}

TEST(Node, metaInt)
{
    IN::NodeIdManager g;
    IN::Node node(&g);
    EXPECT_EQ(node.getInt("age"), 0);
    {
        node.setInt("age", 21);
        EXPECT_EQ(node.getInt("age"), 21);
    }
    {
        node.setInt("age", 22);
        EXPECT_EQ(node.getInt("age"), 22);
    }
}

TEST(Node, getPrimary)
{
    IN::NodeIdManager g;
    IN::Node person(&g);
    
    IN::Node phone(&g);
    IN::TieType ownerType("owner");
    IN::NodeId ownerId =phone.getPrimary(ownerType);
    EXPECT_EQ(ownerId, phone.EMPTY_NODE_ID());
    phone.addTie(ownerType, &person);
    
    ownerId =phone.getPrimary(ownerType.type);
    EXPECT_EQ(ownerId, person.nodeId());
}

TEST(Node, incSeq)
{
    IN::NodeIdManager g;
    IN::Node person(&g);
    
    IN::Node phone(&g);
    IN::TieType ownerType("owner");
    IN::NodeId ownerId =phone.getPrimary(ownerType);
    phone.addTie(ownerType, &person);
    ownerId =phone.getPrimary(ownerType.type);
    
    
    EXPECT_EQ(phone.getSeq(ownerType, person.nodeId()),0);
    phone.incTie(ownerType, &person, 1);
    EXPECT_EQ(phone.getSeq(ownerType, person.nodeId()),1);
    phone.incTie(ownerType, &person, 2);
    EXPECT_EQ(phone.getSeq(ownerType, person.nodeId()),3);
    phone.incTie(ownerType, &person, 20, false);
    EXPECT_EQ(phone.getSeq(ownerType, person.nodeId()),20);

}
TEST(Node, removeTie)
{
    IN::NodeIdManager g;
    IN::Node person(&g);
    
    IN::Node phone(&g);
    IN::TieType ownerType("owner");
    IN::NodeId ownerId =phone.getPrimary(ownerType);
    phone.addTie(ownerType, &person);

    ownerId =phone.getPrimary(ownerType.type);
    EXPECT_EQ(ownerId, person.nodeId());
    phone.deleteTieOutright(IN::Tie(ownerType, ownerId));
    ownerId =phone.getPrimary(ownerType.type);
    EXPECT_EQ(ownerId, IN::Node::EMPTY_NODE_ID());

    
}
