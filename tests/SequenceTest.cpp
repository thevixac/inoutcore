/*SequenceTest.cpp
Sequence
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "Sequence.h"
#include "LocalNodeStore.h"
#include <gtest/gtest.h>


namespace VI = vixac;
namespace IN = VI::inout;
namespace IA = vixac::ina;

TEST(Sequence, aTest)
{
    IA::LocalNodeStore store;
    IA::SequenceAsync seq(&store);
    seq.init([&]{
        store.newNode(IN::NodeType(""),[&](IN::NodeId firstNode){
            store.setStringMeta(firstNode, "comment", "knock knock", [&](){
                seq.addItem(firstNode, 1, [&]{
                    store.newNode(IN::NodeType(""),[&](IN::NodeId secondNode){
                        store.setStringMeta(secondNode, "comment", "whos there", [&](){
                            seq.addItem(secondNode, 2, [&]{
                                seq.all([&](std::vector<IN::NodeId> vec){
                                    ASSERT_EQ(vec.size(), 2);
                                    EXPECT_EQ(vec[0], firstNode);
                                    EXPECT_EQ(vec[1], secondNode);
                                });
                            });
                        });
                    });
                });
            });
        });
    });
}

