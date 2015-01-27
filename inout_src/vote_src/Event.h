/*Event.h
Event
 copyright Vixac Ltd. All Rights Reserved 
*/

#ifndef INCLUDED_EVENT
#define INCLUDED_EVENT



#include "Phenode.h"
#include "Node.h"
#include "NodeStore.h"
#include "DateSpace.h"
#include "Location.h"
#include "User.h"


#include <string>
#include <map>
#include <vector>
namespace vixac
{ 
    namespace inout
    { 
        class Event;


    }
}

class vixac::inout::Event :public vixac::inout::Phenode
{

public:
    Event(vixac::inout::NodeStore * store);
    Event(vixac::inout::NodeId existingId, vixac::inout::NodeStore * store);
    
    Event():isValid_(false){}
    bool isValid() const {return isValid_;}

    void setTitle(std::string const& title);
    //convenience functions that dont use suggestion list
    std::string title() const;
    vixac::inout::DateSpace dateSpace() const;
    vixac::inout::Location location() const;
    std::vector<vixac::inout::User> members() const;
    std::vector<vixac::inout::User> spectators() const;
    
    vixac::inout::Node * getTitleNode() const;
    vixac::inout::Node * getChatNode() const;
    vixac::inout::Node * getPeopleNode() const;
    vixac::inout::Node * getDateSpaceSuggestionNode() const;    
    
    vixac::inout::Node * getWhatList() const;
    vixac::inout::Node * getWhereList() const;
    vixac::inout::Node * getWhenList() const;
    vixac::inout::Node * getNullableWhatNode() const;
private:
    bool isValid_;


};

#endif
