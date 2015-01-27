/*CommentThreadTest.cpp
CommentThread
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "CommentThread.h"

#include <gtest/gtest.h>
#include "UserAsync.h"
#include "LocalNodeStore.h"

namespace VI = vixac;
namespace IN = VI::inout;
namespace IA = vixac::ina;


namespace
{
    IA::UserAsync alice;
    IA::UserAsync bob;
    
}
TEST(CommentThread, aTest)
{
    IN::NodeStore  store;
    IN::CommentThread thread(&store);
    IN::User alice(&store, "alice");
    IN::User bob(&store, "bob");
    IN::User carol(&store, "carol");
    thread.addComment("first", alice);
    thread.addComment("good for you alice, heres a second", bob);
    thread.addComment("well then", carol);
    thread.addComment("good talk.", carol);
    
    std::vector<IN::Comment> comments = thread.commentsBeforeTime(4, store.now()); //TODO time isnt considered yet i guess.
    ASSERT_EQ(comments.size(), 4);
    EXPECT_EQ(comments[0].text(), "first");
    EXPECT_EQ(comments[1].getUserNode()->nodeId(), bob.root()->nodeId());
}

TEST(CommentThread, existingNodeConstructor)
{
    IN::NodeStore  store;
    IN::CommentThread thread(&store);
    IN::User alice(&store, "alice");
    IN::User bob(&store, "bob");
    IN::User carol(&store, "carol");
    thread.addComment("first", alice);
    thread.addComment("good for you alice, heres a second", bob);
    thread.addComment("well then", carol);
    thread.addComment("good talk.", carol);
    
    {
        IN::CommentThread copyThread(thread.root()->nodeId(), &store);
        std::vector<IN::Comment> comments = copyThread.commentsBeforeTime(4, store.now()); //TODO time isnt considered yet i guess.
        ASSERT_EQ(comments.size(), 4);
        EXPECT_EQ(comments[0].text(), "first");
        EXPECT_EQ(comments[1].getUserNode()->nodeId(), bob.root()->nodeId());
    }
}

TEST(CommentThread, asyncTest)
{
    IA::LocalNodeStore store;
    
    IA::CommentThreadAsync thread(&store);
    thread.init([&]{
        alice = IA::UserAsync(&store, "alice");
        alice.init([&]{
            bob =  IA::UserAsync(&store, "bob");
            bob.init([&]{
                thread.addComment("first",alice.rootId(), [&]{
                    thread.addComment("second", bob.rootId(), [&]{
                        thread.commentsBeforeTime(2, 0, [&](std::vector<IN::NodeId> comments){
                            ASSERT_EQ(comments.size(),2);
                        });
                    });
                });
            });
            
        });
    });
}

 