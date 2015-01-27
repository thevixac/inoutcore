/*Name.h
Name
 copyright Vixac Ltd. All Rights Reserved 
*/

#ifndef INCLUDED_NAME
#define INCLUDED_NAME

#include "Phenode.h"
#include "NodeStore.h"
namespace vixac
{ 
    namespace inout
    { 
        class Name;
    }
}

class vixac::inout::Name : public vixac::inout::Phenode
{

public:
    void beTheNameFor(vixac::inout::NodeId node);

    Name(vixac::inout::NodeStore * store);
    Name(vixac::inout::NodeStore * store, std::string const& name);
    Name(vixac::inout::NodeId existing, vixac::inout::NodeStore * store);
    

    void setName(std::string const& name){root()->setString("name", name);}
    std::string name() const {return root()->getString("name");};
private: 

};

#endif
