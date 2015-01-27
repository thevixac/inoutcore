/*Wish.h
Wish
 copyright Vixac Ltd. All Rights Reserved 
*/

#ifndef INCLUDED_WISH
#define INCLUDED_WISH

#include "PhenodeAsync.h"

#include "AsyncFunctor.h"


namespace vixac
{ 
    namespace ina
    { 
        class Wish;
    }
}

class vixac::ina::Wish : public vixac::ina::PhenodeAsync
{

public:
    static void getJobs(NodeStoreAsync * store, vixac::inout::NodeId userNodeId, MetaMapFunc f);
    static void getOutsourced(NodeStoreAsync * store, vixac::inout::NodeId userNodeId, MetaMapFunc f);
    static void createWish(NodeStoreAsync * store, vixac::inout::NodeId creator, BoolFunc f);
    static void getSummary(NodeStoreAsync * store, vixac::inout::NodeId wishId, StringFunc f);

    Wish(){}
    std::string phenodeType() const{return "wish";};
    Wish(NodeStoreAsync * store): vixac::ina::PhenodeAsync(store){}
    Wish(vixac::inout::NodeId nodeId, NodeStoreAsync * store): vixac::ina::PhenodeAsync(nodeId, store){}
    
    

private: 

};



#endif
