/*Sequence.h
Sequence
 copyright Vixac Ltd. All Rights Reserved 
*/

#ifndef INCLUDED_SEQUENCE
#define INCLUDED_SEQUENCE
#include "PhenodeAsync.h"
#include "NodeStoreAsync.h"
#include "GraphUtil.h"
namespace vixac
{ 
    namespace inout
    { 
        class Sequence;
    }
    namespace ina
    {
        class SequenceAsync;
    }
}

class vixac::inout::Sequence
{

public: 

    Sequence();
private: 

};



class vixac::ina::SequenceAsync : public vixac::ina::PhenodeAsync
{
public:
    std::string phenodeType() const{ return type_ == "" ? "sequence" : type_;}
    SequenceAsync(){}
    SequenceAsync(NodeStoreAsync * store): vixac::ina::PhenodeAsync(store){}
    SequenceAsync(vixac::inout::NodeId nodeId, NodeStoreAsync * store): vixac::ina::PhenodeAsync(nodeId, store){}
    
    void addItem(vixac::inout::NodeId, vixac::inout::INEpoch epoch, DoneFunc f);
    void all(NodeIdVecFunc);
    void count(IntFunc f);
    void setPhenodeType(std::string const& type) {type_ = type;};
//    void getMostRecent(size_t quantity, NodeIdVecFunc);
  //  void getItemsAfter(vixac::inout::INEpoch, NodeIdVecFunc);
//    void getItemsBetween(vixac::inout::INEpoch oldest, vixac::inout::INEpoch newest, NodeIdVecFunc);
private:
    std::string type_;
};

#endif
