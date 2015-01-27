/*ProxyNodeTest.cpp
ProxyNode
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "ProxyNode.h"

#include "NodeStore.h"

#include <gtest/gtest.h>


namespace VI = vixac;
namespace IN = VI::inout;

TEST(ProxyNode, aTest)
{
    IN::NodeStore store;
    IN::Node * target = store.newNode();
    target->setString("type", "target");
    
    IN::ProxyNode proxy(&store, target);
    EXPECT_EQ(proxy.proxy()->getString("type"), "target");
}

