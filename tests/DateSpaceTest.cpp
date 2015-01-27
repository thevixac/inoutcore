/*DateSpaceTest.cpp
DateSpace
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "DateSpace.h"

#include <gtest/gtest.h>


namespace VI = vixac;
namespace IN = VI::inout;

TEST(DateSpace, aTest)
{
    //EXPECT_STREQ( "write some tests for DateSpace", "TODO");
    
    IN::DateSpace tenthSecond;
    tenthSecond.setStartEpoch(10, IN::DateResolution::Day);
    IN::DateSpace eleventhSecond;
    eleventhSecond.setStartEpoch(11, IN::DateResolution::Day); //confusing, those numbers are actually seconds
    
    EXPECT_TRUE(tenthSecond < eleventhSecond);
    EXPECT_FALSE(tenthSecond < tenthSecond);
    EXPECT_FALSE(eleventhSecond < tenthSecond);
}

