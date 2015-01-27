/*User.h
User
 copyright Vixac Ltd. All Rights Reserved 
*/

#ifndef INCLUDED_USER
#define INCLUDED_USER


#include <string>
#include "Phenode.h"

namespace vixac
{ 
    namespace inout
    { 
        class User;
//        typedef uint64_t Uuid;
    }
}



class vixac::inout::User : public vixac::inout::Phenode
{

public:
    
    bool isValid() const {return false;}
    User(vixac::inout::Node * node, vixac::inout::NodeStore * store );
    User(vixac::inout::NodeStore * store,std::string const& name = "");
    User(){}
    
    void buildPhenode(std::string const& name);
    std::string name() const;
    vixac::inout::Node * nameNode() const;

private:

};

#endif
