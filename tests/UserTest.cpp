/*UserTest.cpp
User
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "User.h"
#include "Name.h"
#include <gtest/gtest.h>


namespace VI = vixac;
namespace IN = VI::inout;


TEST(User, nameNode)
{
 
    IN::NodeStore store;
    IN::User n(&store, "alice");
    EXPECT_EQ(n.name(), "alice");
    ASSERT_FALSE(n.nameNode() == NULL);
    EXPECT_EQ(IN::Name(n.nameNode()->nodeId(), &store).name(), n.name());
}


TEST(User, injectedRoot)
{
    IN::NodeStore store;
    IN::Node injectedNameRoot;

    IN::NodeId CRUCIAL_NODE_ID = 42;
    injectedNameRoot.switchNodeIdWithGreatCaution(CRUCIAL_NODE_ID);
    store.injectExistingNode(injectedNameRoot);
    ASSERT_FALSE(store.getNode(CRUCIAL_NODE_ID) == NULL);
    injectedNameRoot.switchNodeIdWithGreatCaution(CRUCIAL_NODE_ID);
    IN::User user(store.getNode(CRUCIAL_NODE_ID), &store);
    ASSERT_FALSE(user.root() == NULL);
    EXPECT_TRUE(user.nameNode() == NULL); // this isnt a requirement, more of the problem im about to fix
    
    user.buildPhenode("alice");
    EXPECT_EQ(user.name(), "alice");

}
