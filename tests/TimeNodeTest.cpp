/*TimeNodeTest.cpp
TimeNode
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "TimeNode.h"

#include <gtest/gtest.h>


namespace VI = vixac;
namespace IN = VI::inout;

TEST(TimeNode, aTest)
{

    IN::NodeStore store;
    IN::Node * target = store.newNode();
    target->setString("type", "target");
    IN::TimeNode time(&store, IN::INEpoch(100), IN::TimeResolution::Minute, 60);
    EXPECT_EQ(time.epoch(), IN::INEpoch(100));
    EXPECT_EQ(time.resolution(), IN::TimeResolution::Minute);
    EXPECT_EQ(time.durationInSeconds(), 60);

}

