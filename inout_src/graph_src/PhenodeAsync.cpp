/*PhenodeAsync.cpp
PhenodeAsync
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "PhenodeAsync.h"
#include <iostream>
#include <string>
#include <utility>

namespace VI = vixac;

namespace IA = VI::ina;
namespace IN = vixac::inout;


void IA::PhenodeAsync::init(IA::DoneFunc f)
{
    if(this->rootId()!= IN::Node::EMPTY_NODE_ID()) {
        
        std::cout<< "ERROR. init fail for, root id exists " <<this->rootId() <<this->phenodeType() <<std::endl;
        abort(); //must not call build when building from an existing constructor
    }
    this->build([=](IN::NodeId nodeId){
        
        this->rootId_ = nodeId;
        
        f();
    });
}
void IA::PhenodeAsync::build(IA::NodeIdFunc f)
{
    store_->newNode(IN::NodeType(this->phenodeType()), f);
    
}