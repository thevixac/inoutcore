/*LocalNodeStoreTest.cpp
LocalNodeStore
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "LocalNodeStore.h"
#include "NodeStoreAsync.h"
#include <gtest/gtest.h>

#include <thread>
namespace VI = vixac;

namespace IA = vixac::ina;
namespace IN = VI::inout;

namespace
{
    IA::LocalNodeStore store;
}
TEST(NodeStoreAsync, addTie)
{
    IA::LocalNodeStore store;
    store.newNode(IN::NodeType(""), [&](IN::NodeId nodeId) {
        EXPECT_EQ(nodeId, 1);
        store.newNode(IN::NodeType(""),[&](IN::NodeId secondNodeId){
            EXPECT_EQ(secondNodeId, 2);
            store.addTie(1, IN::Tie(IN::TieType("friend"), 2), [&](){
                std::cout<<"TODO test that the tie is made, without getNode" <<std::endl;
             //   store.getNode(1, [&](IN::Node * target) {
               //     EXPECT_EQ(target->getPrimary(IN::TieType("friend")), 2);
              //  });
            });
        });
    });
}

TEST(NodeStoreAsync, sequence)
{

    
    
    store.newNode(IN::NodeType("BAsE"),[&](IN::NodeId baseNodeId) {
        
        
        store.newNode(IN::NodeType(""), [&](IN::NodeId cure){
            
            std::cout<<"inc " <<baseNodeId <<" and " <<cure <<std::endl;
            store.incSequence(baseNodeId, cure, IN::TieType("best"), 3, false, [&] {
                
                
                store.newNode(IN::NodeType(""), [&](IN::NodeId modest){
                    store.incSequence(baseNodeId, modest, IN::TieType("best"), 1, false, [&] {
                        store.newNode(IN::NodeType(""), [&](IN::NodeId nirvana){
                            store.incSequence(baseNodeId, nirvana, IN::TieType("best"), 2, false, [&] {
                                IA::SequenceRequest req(baseNodeId, IN::TieType("best"));
                                req.sort = IA::SortTypes::Sequence;
                                req.quantity = 0;
                                req.asc =true;
                                
                                store.getActiveTies(baseNodeId, IN::TieType("best"),[&](std::vector<IN::NodeId>vec){
                                     ASSERT_EQ(vec.size(), 3);
                                });

                                store.getSequence(req, [&](std::vector<IN::NodeId>vec){
                                    ASSERT_EQ(vec.size(), 3);
                                    EXPECT_EQ(vec[0], modest);
                                    EXPECT_EQ(vec[1], nirvana);
                                    EXPECT_EQ(vec[2], cure);
                                    
                                    IA::SequenceRequest req2(baseNodeId, IN::TieType("best"));
                                    req2.sort = IA::SortTypes::Sequence;
                                    req2.quantity = 1;
                                    req2.asc = false;
                                    store.getSequence(req2, [&](std::vector<IN::NodeId>vec){
                                        ASSERT_EQ(vec.size(), 1);
                                        EXPECT_EQ(vec[0], cure);
                                    });
                                });
                                
                            });
                 
                        });
          
                    });
                });
        
            });
        });
        });

}