/*VoteListPhenoSpyTest.cpp
VoteListPhenoSpy
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "VoteListPhenoSpy.h"
#include "Vote.h"
#include "Node.h"
#include "User.h"
#include "Name.h"
#include <gtest/gtest.h>


namespace VI = vixac;
namespace IN = VI::inout;

namespace
{
    struct SimpleVoteListSpy : public IN::VoteListPhenoSpy
    {
    public:
        SimpleVoteListSpy():
        numVotesRemoved_(0),
        numVotesAdded_(0),
        numTimesTopChanged_(0),
        numTimesVoteeAdded_(0){}
//        numTimesVoteeRemoved_(0)
        
        void spiedNewVoteeAdded(vixac::inout::Vote const& vote)
        {
            numTimesVoteeAdded_++;
        }
        /*
        void spiedOldVoteeRemoved(vixac::inout::Vote const& vote)
        {
            numTimesVoteeRemoved_++;
        }
         */
        void spiedVoteRemoved(vixac::inout::Vote const& vote)
        {
            numVotesRemoved_++;
            lastVoteRemoved_= vote;
        }
        
        void spiedVoteAdded(vixac::inout::Vote const& vote)
        {
            numVotesAdded_++;
            lastVoteAdded_ = vote;
        }
        
        void spiedTopChangedTo(vixac::inout::Node * node)
        {
            numTimesTopChanged_++;
            lastTop_ = node;
        }
        
        IN::Vote lastVoteRemoved_;
        IN::Vote lastVoteAdded_;
        IN::Node *lastTop_;
        
        int numVotesRemoved_;
        int numVotesAdded_;
        int numTimesTopChanged_;
        int numTimesVoteeAdded_;
     //   int numTimesVoteeRemoved_;
    };
}
TEST(VoteListPhenoSpy, aTest)
{
    IN::NodeStore store;
    IN::VoteList list(&store);
    
    ::SimpleVoteListSpy spy;
    spy.spyOn(&list);
    
    IN::User alice(&store, "alice");
    IN::User bob(&store, "bob");
    IN::User carol(&store, "carol");
    IN::User dan(&store, "dan");
    
    IN::Name  apple(&store, "apple");
    
    IN::Name  banana(&store, "banana");
    
    {
        list.vote(alice, apple.root()->nodeId()); //apple 1A, banana 0
        ASSERT_EQ(spy.numTimesTopChanged_, 1);
        ASSERT_EQ(spy.numVotesAdded_, 1);
        EXPECT_EQ(spy.lastTop_->nodeId(), apple.root()->nodeId());
        EXPECT_EQ(spy.lastVoteAdded_.votee()->nodeId(), apple.root()->nodeId());
        EXPECT_EQ(spy.numTimesVoteeAdded_, 1);
    }
    {
        list.vote(bob, banana.root()->nodeId()); // apple 1A, banana 1B
        EXPECT_EQ(spy.numTimesTopChanged_, 1);
        EXPECT_EQ(spy.numVotesAdded_, 2);
        EXPECT_EQ(spy.lastVoteAdded_.votee()->nodeId(), banana.root()->nodeId());
        EXPECT_EQ(spy.numTimesVoteeAdded_, 2);
    }
    {
        list.vote(carol, banana.root()->nodeId()); // apple 1A, banana 2BC
        EXPECT_EQ(spy.numTimesTopChanged_, 2);
        EXPECT_EQ(spy.numVotesAdded_, 3);
        EXPECT_EQ(spy.lastVoteAdded_.voter()->nodeId(), carol.root()->nodeId());
        EXPECT_EQ(spy.numTimesVoteeAdded_, 2);
    }
    {
        EXPECT_EQ(spy.numTimesTopChanged_, 2);
        list.vote(bob, apple.root()->nodeId()); // apple 2AB, banana 1C
        EXPECT_EQ(spy.numTimesTopChanged_, 3);

        EXPECT_EQ(spy.numVotesAdded_, 4);
        EXPECT_EQ(IN::Name(spy.lastVoteAdded_.votee()->nodeId(), &store).name(), apple.name()) <<" where lastVote added should be bobs changed vote to apple";
        EXPECT_EQ(spy.numVotesRemoved_, 1);
        EXPECT_EQ(spy.lastVoteRemoved_.voter()->nodeId(), bob.root()->nodeId());
        EXPECT_EQ(spy.lastVoteRemoved_.votee()->nodeId(), banana.root()->nodeId());
    }
    {
        list.unvote(bob); ///apple 1A, banana 1C
//        EXPECT_EQ(spy.numTimesTopChanged_, 2);//undfined i guess
        EXPECT_EQ(spy.numVotesRemoved_, 2);
        EXPECT_EQ(spy.lastVoteRemoved_.voter()->nodeId(), bob.root()->nodeId());
        EXPECT_EQ(spy.lastVoteRemoved_.votee()->nodeId(), apple.root()->nodeId());
    }
    
}

