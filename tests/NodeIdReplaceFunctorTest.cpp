/*NodeIdReplaceFunctorTest.cpp
NodeIdReplaceFunctor
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "NodeIdReplaceFunctor.h"

#include <gtest/gtest.h>
#include <vector>

namespace VI = vixac;
namespace IN = VI::inout;

namespace
{
    class SimpleRegistar : public IN::NodeRegistrar
    {
    public:
        SimpleRegistar():start_(10), inc_(3), numReg_(0), numFetch_(0){}
        void registerThisNode(IN::NodeIdReplaceFunctor& f)
        {
            numReg_++;
        }
        void fetchAvailableNodeIds(IN::NodeIdFetch& f)
        {
            f(start_, start_+inc_);
            start_+=inc_;
            numFetch_++;
        }
        int numReg_;
        int numFetch_;
    private:
        IN::NodeId start_;
        IN::NodeId inc_;
    };
    

}
TEST(NodeIdReplaceFunctor, registrar)
{
    //EXPECT_STREQ( "write some tests for NodeIdReplaceFunctor", "TODO");
    IN::NodeStore store;
    ::SimpleRegistar reg;
    IN::StashGenerator gen(&reg);
    store.useThisGenerator(&gen);
    EXPECT_EQ(reg.numReg_, 0);
    EXPECT_EQ(reg.numFetch_, 1);
    
    store.newNode();
    IN::Node * b = store.newNode();
    EXPECT_EQ(b->nodeId(), 12);
    store.newNode();
    EXPECT_EQ(reg.numFetch_, 2);
    IN::Node * d = store.newNode();
    EXPECT_EQ(d->nodeId(), 15);
    store.newNode();
    
}

