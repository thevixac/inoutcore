/*NodeStoreTest.cpp
NodeStore
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "NodeStore.h"
#include "Node.h"
#include "User.h"
#include "NodeIdReplaceFunctor.h"
#include <gtest/gtest.h>
#include <string>

namespace VI = vixac;
namespace IN = VI::inout;
namespace
{
    struct NodeStoreReplicator : public IN::NodeObserver
    {
    public:
        NodeStoreReplicator(IN::NodeStore *base):baseStore_(base)
        {
            base->attachObserverToAll(this);
        }
        

        void nodeGainedTie(IN::Node * node, IN::Tie const& tie)
        {
            IN::Node * clone= this->getClonedNode(node);
            IN::Node * clonedTarget = this->getClonedNode(baseStore_->getNode(tie.target()));
            clone->addTie(tie, clonedTarget);
        }
      
        void nodeRemovedTie(IN::Node * node, IN::Tie const& tie)
        {

            IN::Node * clone= this->getClonedNode(node);
            clone->flagTieForDeletion(tie);
        }
      
        void nodeRecievedTie(IN::Node * node, IN::Tie const& tie, IN::NodeId sourceNode)
        {
           // IN::Node * clone= this->getClonedNode(node);
//            std::cout<<"TODO do i do something here? " <<std::endl;
        }//
      
        void nodeMetaChangedForKey(IN::Node * node, std::string const& key, std::string const& value)
        {
            IN::Node * clone= this->getClonedNode(node);
            clone->setString("key", "value");
        }
        void nodeMetaIntChangedForKey(IN::Node * node, std::string const& key, int64_t value)
        {
            IN::Node * clone= this->getClonedNode(node);
            clone->setInt("key",value);
        }
        void nodeChangedNodeIdFrom(IN::Node * node, IN::NodeId nodeId)
        {
            IN::Node * clone= this->getClonedNode(node);
            clone->switchNodeIdWithGreatCaution(nodeId);
        }
        

        IN::NodeStore * clone() {return &clone_;}
    private:
        IN::Node * getClonedNode(IN::Node * baseNode)
        {
            IN::Node * cloneNode = clone_.getNode(baseNode->nodeId());
            if(cloneNode == NULL)
            {
                IN::Node copy = *baseNode;
                copy.unobserve();
                clone_.injectExistingNode(copy);
            }
            return clone_.getNode(baseNode->nodeId());
        }
        
        IN::NodeStore *const baseStore_;
        IN::NodeStore clone_;
    };
    
    
    
    struct SimpleGenerator : public IN::NodeIdGenerator
    {
        SimpleGenerator():latest_(100){}
        IN::NodeId get()
        {
            latest_+=5;
            return latest_;
        }
        IN::NodeId latest_;
    };
}

TEST(NodeStore, noGeneratorTest)
{
    IN::NodeStore store;
    IN::Node * one= store.newNode();
    IN::Node * two = store.newNode();
    IN::Node * three = store.newNode();
    EXPECT_EQ(one->nodeId(), 1);
    EXPECT_EQ(two->nodeId(), 2);
    EXPECT_EQ(three->nodeId(), 3);
}

TEST(NodeStore, simpleGeneratorTest)
{
    IN::NodeStore store;
    ::SimpleGenerator gen;
    store.useThisGenerator(&gen);
    IN::Node * one= store.newNode();
    IN::Node * two = store.newNode();
    IN::Node * three = store.newNode();
    EXPECT_EQ(one->nodeId(), 105);
    EXPECT_EQ(two->nodeId(), 110);
    EXPECT_EQ(three->nodeId(), 115);
}

TEST(NodeStore, aTest)
{
    const IN::TieType FRIEND_TIE("234");
    IN::NodeStore store;
    IN::NodeId aliceId;
    IN::NodeId carolId;
    {
        IN::Node *alice= store.newNode();
        ASSERT_TRUE(alice != NULL);
        alice->setString("name", "alice");
        aliceId = alice->nodeId();
        ASSERT_FALSE(store.getNode(aliceId) == NULL);
        EXPECT_EQ(store.getNode(aliceId)->getString("name"), "alice");
    }
    {   
        IN::Node *bob= store.newNode();
        bob->setString("name", "bob");
        IN::NodeId bobId = bob->nodeId();
        EXPECT_EQ(store.getNode(bobId)->getString("name"), "bob");
    }
    {
        IN::Node *carol  = store.newNode();
        carol->setString("name", "carol");
        carolId = carol->nodeId();
        EXPECT_EQ(store.getNode(carolId)->getString("name"), "carol");
        IN::Tie carolFriendAliceTie(FRIEND_TIE, carolId);
        store.getNode(aliceId)->addTie(FRIEND_TIE, carol);
    }
}



TEST(NodeStore, newNode)
{
    IN::NodeStore store;
    
    IN::Node * one = store.newNode();
    IN::Node * two = store.newNode();
    EXPECT_EQ(store.size(), 2);
    EXPECT_NE(one->nodeId(), two->nodeId());

}

TEST(NodeStore, carNode)
{
    const IN::TieType OWNERSHIP("ownership");
    const IN::TieType AXELED("axeled");
    const IN::TieType DRIVER("driver");
    const IN::TieType PASSENGER("passenger");
    //TODO finish? what was i getting at
}

TEST(NodeStore, swapNodes)
{
    IN::NodeStore store;
    // one likes two
    // three likes one
    //four likes five

    IN::Node * one = store.newNode();
    IN::Node * two = store.newNode();
    IN::Node * three = store.newNode();
    IN::Node * four = store.newNode();
    IN::Node * five = store.newNode();
    
    IN::TieType likeType("like");
    one->setString("text", "one");
    two->setString("text", "two");
    three->setString("text", "three");
    four->setString("text", "four");
    five->setString("text", "five");
    store.addTie(one->nodeId(), IN::Tie(likeType, two->nodeId(), store.now()));
    store.addTie(three->nodeId(), IN::Tie(likeType, one->nodeId(), store.now()));
    store.addTie(four->nodeId(), IN::Tie(likeType, five->nodeId(), store.now()));
    
    EXPECT_EQ(one->getPrimary(likeType), two->nodeId());
    EXPECT_EQ(four->getPrimary(likeType), five->nodeId());
    EXPECT_EQ(one->getString("text"), "one");
    
    //swap one and four
    
    IN::NodeId oneOldNodeId = one->nodeId();
    IN::NodeId fourOldNodeId = four->nodeId();
    store.swapNodeIds(one->nodeId(), four->nodeId());

    //The freaking memeory is being swapped. Node * value doesnt change, but it now points to a different node can i count on this to always be true?

    //check this isnt a coincidence.
    IN::Node * newlyGottenOne =store.getNode(oneOldNodeId);
    

    EXPECT_EQ(one->nodeId(), store.getNode(one->nodeId())->nodeId());
    EXPECT_EQ(newlyGottenOne->nodeId(), store.getNode(newlyGottenOne->nodeId())->nodeId());
    
   
     //therein lies the fuckup. The nodes have been swapped in memory in nodestores nodes_, so i cant actually talk to four anymore, because four is now pointing to the node formerly known as one.
    /*
    EXPECT_EQ(oneOldNodeId, newlyGottenFour->nodeId());
    EXPECT_EQ(oneOldNodeId, four->nodeId());
    EXPECT_EQ(fourOldNodeId, newlyGottenOne->nodeId());
    */

    
    EXPECT_EQ(one->getPrimary(likeType), five->nodeId());
    EXPECT_EQ(four->getPrimary(likeType), two->nodeId());
    EXPECT_EQ(newlyGottenOne->getString("text"), "four");
    
    //TODO more thorough tests of swapnode please. Does it work in every sense?

}

TEST(wtf, thisIsWhyThePreviousTestIsSoConfusing)
{
    std::string one("one");
    std::string two("two");
    std::map<int, std::string *> map;
    
    map[1] = &one;
    map[2] = &two;
    std::map<int, std::string *>::iterator it = map.find(1);
    EXPECT_EQ(*(it->second), "one");
    
    map[1] = map[2];
    EXPECT_EQ(*(it->second), "two");
}

TEST(NodeStore, injectExistingNode)
{
    
    IN::NodeStore store;
    IN::Node * one = store.newNode();
    
    
    IN::Node * another = store.newNode();
    one->addTie(IN::TieType("friend"), another);
    one->setInt("age", 21);
    IN::NodeStore secondStore;
    secondStore.injectExistingNode(*store.getNode(one->nodeId()));
    
    IN::Node *oneCopy  = secondStore.getNode(one->nodeId());
    std::cout<<"TODO test injectexisting node" <<std::endl;
    std::map<IN::TieType, std::vector<IN::Tie> > all = one->allTies();
    std::map<IN::TieType, std::vector<IN::Tie> > allCopy =oneCopy->allTies();
    ASSERT_EQ(all.size(), allCopy.size());
    std::vector<IN::Tie> oneTies = all[IN::TieType("friend")];
    std::vector<IN::Tie> copyTies = allCopy[IN::TieType("friend")];
    EXPECT_EQ(oneTies.size(),1);
    EXPECT_EQ(copyTies.size(), oneTies.size());
    EXPECT_EQ(oneTies[0].target(), another->nodeId());
    EXPECT_EQ(oneTies[0].target(), copyTies[0].target());
    
}


TEST(NodeStore, NodeStoreReplicator)
{
    IN::NodeStore store;

    IN::Node * one = store.newNode();
    IN::Node * two = store.newNode();
    IN::Node * three = store.newNode();
    
    one->setString("text", "one");
    two->setString("text", "two");
    three->setString("text", "three");
    
    IN::TieType likeType("like");
    store.addTie(one->nodeId(), IN::Tie(likeType, two->nodeId(), store.now()));
    store.addTie(three->nodeId(), IN::Tie(likeType, one->nodeId(), store.now()));

    ::NodeStoreReplicator replicator(&store);
    IN::Node * four = store.newNode();
    IN::Node * five = store.newNode();

    four->setString("text", "four");
    five->setString("text", "five");
    store.addTie(four->nodeId(), IN::Tie(likeType, five->nodeId(), store.now()));
    store.addTie(one->nodeId(), IN::Tie(likeType, five->nodeId(), store.now()));
    store.addTie(five->nodeId(), IN::Tie(likeType, one->nodeId(), store.now()));
    
    EXPECT_EQ(replicator.clone()->getNode(one->nodeId())->getString("text"), "one");
    EXPECT_EQ(replicator.clone()->getNode(one->nodeId())->getPrimary(IN::TieType("like")), two->nodeId());

}


TEST(NodeStore, moveNode)
{
    IN::NodeStore store_;
    IN::User alice(&store_, "alice");
    IN::User bob(&store_, "bob");
    IN::User carol(&store_, "carol");
    alice.root()->setInt("age", 21);
    bob.root()->setInt("age", 22);
    carol.root()->setInt("age", 23);
    alice.root()->addTie(IN::TieType("friend"), bob.root());
    
    
    IN::NodeId oldBobId = bob.root()->nodeId();
    IN::NodeId newBobId = 1000;
    store_.moveNode(bob.root(), newBobId);
    
    alice.root()->addTie(IN::TieType("employee"), carol.root());
    carol.root()->addTie(IN::TieType("brother"), bob.root());
    bob.root()->addTie(IN::TieType("sister"), carol.root());
    EXPECT_EQ(store_.getNode(alice.root()->getPrimary(IN::TieType("employee")))->nodeId(), carol.root()->nodeId());
    EXPECT_EQ(store_.getNode(alice.root()->getPrimary(IN::TieType("friend")))->nodeId(), bob.root()->nodeId()); //this would be the old bobId so we get oldBobid from store are retrieve new
    EXPECT_EQ(bob.root()->getPrimary(IN::TieType("sister")), carol.root()->nodeId());
    EXPECT_EQ(carol.root()->getPrimary(IN::TieType("brother")), bob.root()->nodeId()); //this happened after the move so its actually the right value.
    EXPECT_EQ(store_.getNode(oldBobId)->nodeId(), newBobId);
    EXPECT_EQ(store_.getNode(newBobId)->getInt("age"), 22);
}

TEST(NodeStore, nodestoreCloneUser)
{
    IN::NodeStore store_;
    ::NodeStoreReplicator replicator(&store_);
    IN::User alice(&store_, "alice");
    IN::NodeId aliceId = alice.root()->nodeId();

    alice.root()->setInt("age", 21);
    
    
    //TODO legitmate bug here.
    std::cout<<"TODO if i use the replicator, these tests start failing, and mysteriously alicecopy gets 2 name ties" <<std::endl;
    IN::NodeStore storeCopy = * replicator.clone();
    IN::NodeStore storeCopy2;
    std::map<IN::NodeId,IN::Node> allNodes = store_.allNodes();
    for(std::map<IN::NodeId,IN::Node>::const_iterator it = allNodes.begin(), end = allNodes.end(); it != end; ++it)
    {
        EXPECT_EQ(it->first, it->second.nodeId()); //TODO pointless test no?
        storeCopy2.injectExistingNode(it->second);
    }
//    IN::User aliceCopy(replicator.clone()->getNode(aliceId), replicator.clone());
    //IN::User aliceCopy(store_.getNode(aliceId), &store_);
    IN::User aliceCopy(storeCopy2.getNode(aliceId), &storeCopy2);
    
   // IN::User aliceAgain(store_)

    EXPECT_EQ(store_.size(), replicator.clone()->size());
    EXPECT_EQ(store_.size(), storeCopy2.size());
    EXPECT_EQ(aliceCopy.root()->nodeId(), alice.root()->nodeId());
    
    IN::TieTypeIteratorConst it = aliceCopy.root()->getTies(IN::TieType("name"));
    std::vector<IN::Tie> ties = it->second;
    for(int i=0; i< ties.size(); i++)
    {
        std::cout<<"ties " <<i << " is " << ties[i].target() <<std::endl;
    }
    EXPECT_EQ(aliceCopy.root()->allIntMeta().size(), alice.root()->allIntMeta().size());

    EXPECT_EQ(alice.name(), "alice");
    EXPECT_EQ(aliceCopy.name(), alice.name());
    EXPECT_EQ(aliceCopy.root()->getTies(IN::TieType("name"))->second.size(), alice.root()->getTies(IN::TieType("name"))->second.size());
    EXPECT_EQ(aliceCopy.root()->getPrimary(IN::TieType("name")), alice.root()->getPrimary(IN::TieType("name")));
    EXPECT_EQ(aliceCopy.root()->getPrimary(IN::TieType("employee")), alice.root()->getPrimary(IN::TieType("employee")));
    EXPECT_EQ(aliceCopy.root()->getInt("age"), alice.root()->getInt("age"));
//    EXPECT_EQ(aliceCopy.name(), alice.name());
    std::cout<<"SO THE LOCAL COPY ALICE IS " <<aliceCopy.name() <<",actual is " <<alice.name()<<std::endl;
}
