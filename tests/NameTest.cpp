/*NameTest.cpp
Name
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "Name.h"
#include "NodeStore.h"
#include <gtest/gtest.h>


namespace VI = vixac;
namespace IN = VI::inout;

TEST(Name, aTest)
{
    IN::NodeStore store;
    IN::Name empty(&store);
    
    EXPECT_EQ(empty.name(), "");
    empty.setName("vic");
    EXPECT_EQ(empty.name(), "vic");
    
    IN::Name copy(empty.root()->nodeId(), &store);
    EXPECT_EQ(copy.name(), "vic");
    
}

TEST(Name, beTheNameFor)
{
    IN::NodeStore store;
    IN::Name name(&store, "alice");
    
    IN::Node *alice = store.newNode();
    
//    EXPECT_EQ(store.getNode(alice->nodeId())->nodeId(), alice->nodeId());
    name.beTheNameFor(alice->nodeId());
    
    EXPECT_EQ(alice->getPrimary(IN::TieType("name")), name.root()->nodeId());
    
    
}
