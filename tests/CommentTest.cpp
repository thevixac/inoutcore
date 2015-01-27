/*CommentTest.cpp
Comment
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "Comment.h"

#include <gtest/gtest.h>
#include "LocalNodeStore.h"


namespace VI = vixac;
namespace IN = VI::inout;
namespace IA = vixac::ina;
TEST(Comment, aTest)
{
   // EXPECT_STREQ( "write some tests for Comment", "TODO");
}
TEST(Comment, asyncTest)
{
    IA::LocalNodeStore store;
    
    store.newNode(IN::NodeType("comment"),[&](IN::NodeId userNodeId){
        IA::CommentAsync comment(&store, userNodeId,"test comment");
        comment.init([&](){
            EXPECT_EQ(comment.rootId(), 2);
            comment.text([&](std::string str){
                EXPECT_EQ(str, "test comment");
                comment.author([&](IN::NodeId authorNodeId){
                    EXPECT_EQ(authorNodeId, userNodeId);
                });
            });
        });
    });
    
    
}

