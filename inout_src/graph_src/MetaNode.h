/*MetaNode.h
MetaNode
 copyright Vixac Ltd. All Rights Reserved 
*/

#ifndef INCLUDED_METANODE
#define INCLUDED_METANODE



#include <map>
#include <string>
namespace vixac
{ 
    namespace inout
    { 
        class MetaNode;
    }
}

//TODO im not using this either 
class vixac::inout::MetaNode
{

public: 

    MetaNode();
    
    std::string getString(std::string const& string) const {
        std::map<std::string, std::string>::const_iterator it = data_.find(string);
        if(it == data_.end())
        {
            return  std::string("");
        }
        return it->second;
    }
private:
    std::map<std::string, std::string> data_; //TODO room for optimisation

};

#endif
