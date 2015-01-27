/*Sequence.cpp
Sequence
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "Sequence.h"
#include <iostream>
#include <string>

namespace VI = vixac;
namespace IN = VI::inout;
namespace IA = vixac::ina;


void IA::SequenceAsync::addItem(vixac::inout::NodeId nodeId,IN::INEpoch epoch, IA::DoneFunc f)
{
    store_->addTie(this->rootId(), IN::Tie( IN::TieType("sequence_item"), nodeId, epoch), f);
}

void IA::SequenceAsync::count(IA::IntFunc f)
{
    //TODO the store should help with this
    this->all([=](std::vector<IN::NodeId> vec)
    {
        f(vec.size());
    });
}

void IA::SequenceAsync::all(IA::NodeIdVecFunc f)
{
    store_->getActiveTies(this->rootId(), IN::TieType("sequence_item"), f);
}
