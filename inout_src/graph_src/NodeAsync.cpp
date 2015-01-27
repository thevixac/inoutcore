/*NodeAsync.cpp
NodeAsync
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "NodeAsync.h"
#include <iostream>
#include <string>

namespace VI = vixac;
namespace IA = VI::ina;
namespace IN = vixac::inout;

 

void IA::NodeAsync::addTie(IN::TieType type, IN::NodeId targetId,  IA::DoneFunc f, bool notifyTarget)
{
    store_->addTie(nodeId_, IN::Tie(type, targetId), f); //TODO if notifyTarget,  pass in a lambda that sets the second one first
}

void IA::NodeAsync::setStringMeta(std::string const& key, std::string const& value, IA::DoneFunc f)
{
    //store_->getNode(nodeId_, [&](IA::NodeAsync n){
  //      n.setStringMeta(key, value, f);
  //  });
}

void IA::NodeAsync::setIntMeta(std::string const& key, int64_t value, IA::DoneFunc f)
{
   // store_->getNode(nodeId_, [&](IA::NodeAsync n){
//n.setIntMeta(key, value, f);
///
   // });
}

void IA::NodeAsync::getStringMeta(std::string const& key, IA::StringFunc f)
{
    //store_->getNode(nodeId_, [&](IA::NodeAsync n){
   //     n.getStringMeta(key, f);
  //  });
}

void IA::NodeAsync::getIntMeta(std::string const& key, IA::IntFunc f)
{
  //  store_->getNode(nodeId_, [&](IA::NodeAsync n){
 //       f(n.getInt(key));
  //  });
}

void IA::NodeAsync::getPrimary(IN::TieType type, IA::NodeIdFunc f)
{
  //  store_->getNode(nodeId_, [&](IA::NodeAsync n){
  //      f(n.getPrimary(type));
  //  });
}

void IA::NodeAsync::getFloatMeta(const std::string &key, FloatFunc f)
{
    
}
void IA::NodeAsync::setFloatMeta(std::string const& key, double value, DoneFunc f)
{
    
}
void IA::NodeAsync::getActiveTies(IN::TieType type, IA::NodeIdVecFunc f)
{
    /*
    store_->getNode(nodeId_, [&](IA::NodeAsync n){
        std::vector<IN::Tie> ties =n->getActiveTies(type);
        std::vector<IN::NodeId> nodeIds;
        for (int i=0;i < ties.size(); i++)
        {
            nodeIds.push_back(ties[i].target());
        }
        f(nodeIds);
    });
     */
}