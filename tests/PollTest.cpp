/*PollTest.cpp
Poll
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "Poll.h"

#include "LocalNodeStore.h"
#include <gtest/gtest.h>


namespace VI = vixac;
namespace IA = VI::ina;
namespace  IN = vixac::inout;

TEST(Poll, onePersonsVote)
{
    IA::LocalNodeStore store;
    IN::NodeType PLACE("place");
    IN::NodeType PERSON("person");
    store.newNode(PLACE, [&](IN::NodeId pub){
        store.newNode(PLACE, [&](IN::NodeId home){
                store.newNode(PERSON, [&](IN::NodeId alice){
                    IA::Poll poll(&store);
                    poll.init([&]() {
                        poll.vote(alice, pub, [&]{
                            poll.topVote([&](IN::NodeId top){
                                EXPECT_EQ(top, pub);
                                poll.vote(alice, home, [&]{
                                    poll.topVote([&](IN::NodeId secondTop){
                                        EXPECT_EQ(secondTop, home);
                                        poll.personsVote(alice, [&](IN::NodeId alicesVote){
                                            EXPECT_EQ(alicesVote, home);
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

TEST(Poll, twoVoters)
{
    IA::LocalNodeStore store;
    IN::NodeType PLACE("place");
    IN::NodeType PERSON("person");
    store.newNode(PLACE, [&](IN::NodeId pub){
        store.newNode(PLACE, [&](IN::NodeId home){
            store.newNode(PERSON, [&](IN::NodeId alice){
                store.newNode(PERSON, [&](IN::NodeId bob){
                     store.newNode(PERSON, [&](IN::NodeId carol){
                         IA::Poll poll(&store);
                         poll.init([&]() {
                             poll.vote(alice, pub, [&]{
                                 poll.vote(bob, home, [&]{
                                     poll.vote(carol, home, [&]{
                                         poll.topVote([&](IN::NodeId top){
                                             EXPECT_EQ(top, home);
                                             poll.personsVote(alice, [&](IN::NodeId alicesVote){
                                                 EXPECT_EQ(alicesVote, pub);
                                             });
                                         });
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

