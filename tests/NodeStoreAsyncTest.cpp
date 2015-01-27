/*NodeStoreAsyncTest.cpp
NodeStoreAsync
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "NodeStoreAsync.h"


#include "Node.h"
#include "NodeGenAsync.h"
#include "LocalNodeStore.h"

#include <gtest/gtest.h>
#include <functional>

namespace VI = vixac;
namespace IA = VI::ina;
namespace IN = vixac::inout;

namespace
{
    class Foo
    {
      
    public:
        Foo(int base):base_(base){}
        int dub(int x){return x*2 + base_;}
        int base_;
    };
    
    class Bar
    {
    public:
        int redub(std::function<int(int)> f)
        {
            int v = f(20);
            return v +=1;
        }
    };
    
    class SimpleAsyncStore :  public vixac::ina::NodeStoreAsync
    {
    public:
        int doNothing_;
        std::map<vixac::inout::NodeId, std::set<IA::NodeAsyncObserver *> >* observers(){
            return &observers_;
        }

    };
    class SimpleNodeObserver : public vixac::ina::NodeAsyncObserver
    {
    public:
        SimpleNodeObserver():tg(0),sc(0),trem(0),trec(0),ic(0),nc(0){}
        void tieGained(IA::TieChange  t)
        {
            tg++;
            ltg = t;
        }
        void stringChanged(IA::StringChange  t)
        {
            sc++;
            lsc=t;
        }
        
        void tieRemoved(IA::TieChange  t)
        {
            trem++;
            ltrem = t;
        }
        void tieRecieved(IA::TieChange t)
        {
            trec++;
            ltrec = t;
        }
        
        void intChanged(IA::IntChange t)
        {
            ic++;
            lic = t;
        }
        void nodeIDChanged(IA::NodeIdChange t)
        {
            nc++;
            lnc =t;
        }
        
        ~SimpleNodeObserver() {
        }
        int tg;
        int sc;
        int trem;
        int trec;
        int ic;
        int nc;
        IA::TieChange ltg;
        IA::StringChange lsc;
        IA::TieChange ltrem;
        IA::TieChange ltrec;
        IA::IntChange lic;
        IA::NodeIdChange lnc;
        
    };

}


TEST(NodeStoreAsync, observers)
{
    ::SimpleAsyncStore store;
    ::SimpleNodeObserver oneObs;
    ::SimpleNodeObserver twoObs;
    ::SimpleNodeObserver threeObs;
    ::SimpleNodeObserver oneAndTwoObs;
    
    IN::NodeId NODE_ONE(1);
    IN::NodeId NODE_TWO(2);
    IN::NodeId NODE_THREE(3);
    store.subscribeToNode(NODE_ONE, &oneObs);
    store.subscribeToNode(NODE_TWO, &twoObs);
    store.subscribeToNode(NODE_THREE, &threeObs);

    store.subscribeToNode(NODE_ONE, &oneAndTwoObs);
    store.subscribeToNode(NODE_TWO, &oneAndTwoObs);
    

    store.tieGained(IA::TieChange(NODE_ONE,IN::Tie(IN::TieType("friend"), NODE_TWO)));
    {
        EXPECT_EQ(oneObs.tg, NODE_ONE);
        EXPECT_EQ (oneObs.ltg.tie.target(), NODE_TWO);
        EXPECT_EQ(oneObs.ltg.tie.type().type, "friend");
    }
    {
        EXPECT_EQ(twoObs.trec, 1);
    }
    {
        store.stringChanged(IA::StringChange(NODE_ONE, "name", "alice"));
        EXPECT_EQ(oneObs.sc, 1);
        EXPECT_EQ(oneObs.lsc.key, "name");
        EXPECT_EQ(oneObs.lsc.value, "alice");
    }


}

//proof that i can gtest C++11 calls and bind member functions
TEST(cpp11test, aTest)
{
    ::Foo fooTen(10);
    std::function<int(int)> func = std::bind(& ::Foo::dub, &fooTen, std::placeholders::_1);
    int v = func(3);//3 *2 +10 = 16;
    EXPECT_EQ(v, 16);
    ::Bar bar;
    EXPECT_EQ(bar.redub(func), 51);
    
}

TEST(NodeStoreAsync, observersLifetimes)
{
    ::SimpleAsyncStore store;
    std::map<vixac::inout::NodeId, std::set<IA::NodeAsyncObserver *> >*  observers =store.observers();
    SimpleNodeObserver oneAndTwoObs;

    IN::NodeId NODE_ONE(1);
    {
        ::SimpleNodeObserver oneObs;
        EXPECT_EQ(observers->size(),0);
        store.subscribeToNode(NODE_ONE, &oneObs);
        ASSERT_TRUE(observers->find(NODE_ONE)!= observers->end());;
        EXPECT_EQ(observers->find(NODE_ONE)->second.size(),1);
    }
    EXPECT_EQ(observers->find(NODE_ONE)->second.size(),0);
    
}


