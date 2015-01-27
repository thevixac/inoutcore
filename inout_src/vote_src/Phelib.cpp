/*Phelib.cpp
Phelib
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "Phelib.h"
#include <iostream>
#include <string>
#include <sstream>
namespace VI = vixac;
namespace IN = vixac::inout;
namespace IA = VI::ina;

IN::TieType IA::Phelib::fixate(IN::TieType tie, IN::NodeId nodeId)
{
    std::string str = tie.type;
    std::stringstream s;
    s <<str;
    s<<"_";
    s<<nodeId;
    return s.str();
}

