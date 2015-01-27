/*ProxyNode.h
ProxyNode
 copyright Vixac Ltd. All Rights Reserved 
*/

#ifndef INCLUDED_PROXYNODE
#define INCLUDED_PROXYNODE

#include "Phenode.h"
#include "NodeStore.h"
namespace vixac
{ 
    namespace inout
    { 
        class ProxyNode;
    }
}

class vixac::inout::ProxyNode : public vixac::inout::Phenode
{
public: 
    ProxyNode(vixac::inout::NodeStore * store, vixac::inout::Node * target); // create a new proxy with the target
    ProxyNode(vixac::inout::NodeId existing, vixac::inout::NodeStore * store);
    
    vixac::inout::Node * proxy() const;
    
    //TODO this is actually difficult, need to swap nodeIds or something with another proxy.
private:


};

#endif
