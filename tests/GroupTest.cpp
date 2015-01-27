/*GroupTest.cpp
Group
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "Group.h"
#include "LocalNodeStore.h"
#include <gtest/gtest.h>


namespace VI = vixac;
namespace IN = vixac::inout;
namespace IA = VI::ina;

TEST(Group, aTest)
{
    IA::LocalNodeStore store;
    
    IA::Group group(&store);
    group.init([&]{
        group.getName([&](std::string name){
            EXPECT_EQ(name, "");
        });
        store.newNode(IN::NodeType("group"),[&](IN::NodeId userNodeId){
            group.addMember(userNodeId, [&]{
                group.getMembers([&](std::vector<IN::NodeId> nodeIds){
                    ASSERT_EQ(nodeIds.size(), 1);
                    EXPECT_EQ(nodeIds[0], userNodeId);
                });
            });
        });
 
    });
 }



//TODO this is broken i thnik because of the occasional phenode api in group. TODO fix
/*

TEST(Group, spyTest)
{
    IA::LocalNodeStore store;
    
    IA::Group group(&store);
    
    IA::GroupSpy spy;
    int numNameChanges= 0;
    std::string curName ="";
    group.init([&]{
        spy.spyOnGroup(group);
        EXPECT_EQ(numNameChanges, 0);
        spy.spyOnName([&](std::string newName){
            numNameChanges++;
            curName = newName;
        });
        EXPECT_EQ(numNameChanges, 1);
        group.setName("firstName", [&]{
            EXPECT_EQ( numNameChanges, 2);
            EXPECT_EQ(curName, "firstName");
            group.setName("secondName", [&] {
                EXPECT_EQ(numNameChanges, 3);
                EXPECT_EQ(curName, "secondName");
   
                store.getPrimary(0, IN::TieType("grp_update"), [&](IN::NodeId n){
                    EXPECT_EQ(n, group.rootId());
                });

            });
            
        });
       
        
    });
}
*/

//TODO port over to getComments
/*

TEST(Group, comment)
{
    IA::LocalNodeStore store;
    
    IA::Group group(&store);
    IA::GroupSpy spy;
    
    IN::NodeId commentId=-1;
    int numCommentsSpied =0;
    group.init([&]{
        spy.spyOnGroup(group);

        group.getChatNode([](IN::NodeId chatNodeId){
            EXPECT_TRUE(chatNodeId != IN::Node::EMPTY_NODE_ID());
        });
        spy.spyOnChat([&](IN::NodeId chatId) {
            std::cout<<"spied something! " <<std::endl;
            numCommentsSpied++;
            EXPECT_EQ(chatId, commentId);
        });
        
        IA::CommentAsync c(&store, 1, "hello");
        c.init([&]{
            commentId = c.rootId();
            group.publishComment(c.rootId(), [&] {
                std::cout<<"published comment" <<std::endl;
                group.getChatThread([&](std::vector<IN::NodeId> vec) {
                    ASSERT_EQ(vec.size(),1);
                    IA::CommentAsync retrievedComment(vec[0], &store);
                    retrievedComment.text([&](std::string str){
                        EXPECT_EQ(str, "hello");
                        EXPECT_EQ(numCommentsSpied, 1);
                    });
                });
            });
        });
    });
    
}
*/