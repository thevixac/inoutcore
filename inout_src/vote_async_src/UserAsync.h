/*UserAsync.h
UserAsync
 copyright Vixac Ltd. All Rights Reserved 
*/

#ifndef INCLUDED_USERASYNC
#define INCLUDED_USERASYNC


#include "AsyncFunctor.h"
#include "NodeStoreAsync.h"
#include "PhenodeAsync.h"
#include "AsyncFunctor.h"
#include <string>

namespace vixac
{ 
    namespace ina
    { 
        class UserAsync;
    }
}

class vixac::ina::UserAsync : public vixac::ina::PhenodeAsync
{
public:
    std::string phenodeType() const{ return "user";}
    UserAsync():str_(""){}
    UserAsync(NodeStoreAsync * store,std::string const& str ="") : vixac::ina::PhenodeAsync(store), str_(str){}

    UserAsync(vixac::inout::NodeId nodeId, NodeStoreAsync * store):vixac::ina::PhenodeAsync(nodeId, store), str_(""){}
    
    void name(StringFunc  f) const;
    void setName(std::string const& str, DoneFunc  f) const;
    void getMobile(StringFunc f);
    void nameNode(NodeIdFunc f);
    
    void getGroups(NodeIdVecFunc f);
    
    void getGroupDetail(MetaMapFunc f);
    
    void setScreen(std::string const& str, DoneFunc f) const;
private:
    void setName(std::string const& str, vixac::inout::NodeId rootNodeId, DoneFunc f) const;
    std::string str_;
    void build(NodeIdFunc);


};

#endif
