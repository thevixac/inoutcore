/*Event.cpp
Event
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "Event.h"
#include "Name.h"
#include "CommentThread.h"
#include "VoteList.h"
#include <iostream>
#include <string>

namespace VI = vixac;
namespace IN = VI::inout;

IN::Event::Event(IN::NodeStore * store)
:IN::Phenode(store->newNode(), store),
isValid_(true)
{
    {
        IN::Name name(store);
        name.beTheNameFor(this->root()->nodeId());
    }
    
    //TODO generalise this, much?
    {
        IN::CommentThread thread(store);
        IN::TieType mainChatType("main_chat");
        store->addTie(this->root()->nodeId(), IN::Tie(mainChatType, thread.root()->nodeId(), store->now()));
    }
    {
        IN::VoteList whatList(store);
        IN::TieType whatListType("what_list");
        store->addTie(this->root()->nodeId(), IN::Tie(whatListType, whatList.root()->nodeId(), store->now()));
      
    }
    {
        IN::VoteList whereList(store);
        IN::TieType whereListType("where_list");
        store->addTie(this->root()->nodeId(), IN::Tie(whereListType, whereList.root()->nodeId(), store->now()));
    }
    {
        IN::VoteList whatList(store);
        IN::TieType whatListType("when_list");
        store->addTie(this->root()->nodeId(), IN::Tie(whatListType, whatList.root()->nodeId(), store->now()));
        
    }
    
}

//TODO use it i guess
IN::Event::Event(IN::NodeId existingId, IN::NodeStore * store)
: IN::Phenode(store->getNode(existingId), store),
isValid_(true)
{
    
}



IN::Node * IN::Event::getTitleNode() const
{
    return store_->getNode(this->root()->getPrimary(IN::TieType("name")));
}

void IN::Event::setTitle(std::string const& title)
{
    IN::Name name(this->root()->getPrimary(IN::TieType("name")), store_);
    name.setName(title);
}

std::string IN::Event::title() const
{
    return IN::Name(this->root()->getPrimary(IN::TieType("name")), store_).name();

}
IN::Node * IN::Event::getChatNode() const
{
    return store_->getNode(this->root()->getPrimary(IN::TieType("main_chat")));
}
IN::Node * IN::Event::getPeopleNode() const
{
    return NULL;
}
IN::Node * IN::Event::getDateSpaceSuggestionNode() const
{
    return NULL;
}

IN::Node * IN::Event::getWhatList() const
{
    return store_->getNode(this->root()->getPrimary(IN::TieType("what_list")));
}

IN::Node * IN::Event::getWhereList() const
{
    return store_->getNode(this->root()->getPrimary(IN::TieType("where_list")));
}

IN::Node * IN::Event::getWhenList() const
{
    return store_->getNode(this->root()->getPrimary(IN::TieType("when_list")));
}

IN::Node * IN::Event::getNullableWhatNode() const
{
    return (IN::VoteList(store_->getNode(this->root()->getPrimary(IN::TieType("what_list")))->nodeId(), store_).topVoteeNullable());
}
