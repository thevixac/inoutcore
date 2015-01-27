/*UserAsyncTest.cpp
UserAsync
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "UserAsync.h"

#include <gtest/gtest.h>


#include "LocalNodeStore.h"

namespace VI = vixac;
namespace IA = vixac::ina;
namespace IN = VI::inout;

TEST(UserAsync, aTest)
{
  
    IA::LocalNodeStore store;
    IA::UserAsync user(&store);
    user.init([&](){
        user.name([&](std::string str) {
            EXPECT_EQ(str, "");
            user.setName("vic", [&](){
                user.name([&](std::string str) {
                    EXPECT_EQ(str, "vic");
                });
            });
        });
    });
}


//TODO test passing in str into the constructor instead of setName
TEST(UserAsync, twoPhenodesSameRoot)
{
    IA::LocalNodeStore store;
    IA::UserAsync user(&store);
    user.init([&](){
        EXPECT_EQ(user.rootId(), 1);
        user.setName("vic", [&]{
            IA::UserAsync copy(user.rootId(), &store);
            copy.name([&](std::string str) {
                EXPECT_EQ(str,"vic");
                copy.setName("bob", [&](){
                    user.name([](std::string originalNameAfterCopyChangedIt) {
                        EXPECT_EQ(originalNameAfterCopyChangedIt, "bob");
                    });
                });
            });
        });
    });
}
