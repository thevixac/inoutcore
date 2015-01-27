/*Phelib.h
Phelib
 copyright Vixac Ltd. All Rights Reserved 
*/

#ifndef INCLUDED_PHELIB
#define INCLUDED_PHELIB

#include "GraphUtil.h"
#include "Tie.h"
namespace vixac
{ 
    namespace ina
    { 
        class Phelib;
    }
}

class vixac::ina::Phelib
{

public:
    static vixac::inout::TieType fixate(vixac::inout::TieType tie, vixac::inout::NodeId nodeId);


};

#endif
