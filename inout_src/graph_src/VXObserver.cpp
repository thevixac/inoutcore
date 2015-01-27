/*VXObserver.cpp
VXObserver
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "VXObserver.h"
#include <iostream>
#include <string>
#include <algorithm>

namespace VI = vixac;
namespace IN = VI::inout;




void IN::VXObservable::removeObserver(IN::VXObserver * observer)
{
    observers_.erase(std::remove(observers_.begin(), observers_.end(), observer), observers_.end());
}
void IN::VXObservable::addObserver(IN::VXObserver * observer)
{
    observers_.push_back(observer);
}
