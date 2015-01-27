/*DateSpace.cpp
DateSpace
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "DateSpace.h"
#include <iostream>
#include <string>

namespace VI = vixac;
namespace IN = VI::inout;

IN::DateSpace::DateSpace()
:duration_(0),
startEpoch_(0)
{
}

void IN::DateSpace::setStartEpoch(int64_t start, IN::DateResolution::Type resolution)
{
    startEpoch_ = start;
    resolution_ = resolution;
}

void IN::DateSpace::setDuration(int64_t seconds)
{
    duration_ = seconds;
}

bool IN::DateSpace::operator<(IN::DateSpace const& rhs)
{
    return startEpoch_ < rhs.start();
}