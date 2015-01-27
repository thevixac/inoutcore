/*Group.h
Group
 copyright Vixac Ltd. All Rights Reserved 
*/

#ifndef INCLUDED_GROUP
#define INCLUDED_GROUP


#include "PhenodeAsync.h"

#include "AsyncFunctor.h"
#include "Sequence.h"
#include "Comment.h"
namespace vixac
{ 
    namespace ina
    {
        class GroupBase;
        class Group;
        class GroupSpy;
    }
}

class vixac::ina::Group : public vixac::ina::PhenodeAsync
{

public: 
    Group(){}
    std::string phenodeType() const{return "group";};

    Group(NodeStoreAsync * store): vixac::ina::PhenodeAsync(store){}
    Group(vixac::inout::NodeId nodeId, NodeStoreAsync * store): vixac::ina::PhenodeAsync(nodeId, store){}
    
    std::vector<std::string> getCommentFields() const;
    void setActiveUser(vixac::inout::NodeId userId);
    void setCreator(vixac::inout::NodeId, DoneFunc f);
    void setName(std::string const& name, DoneFunc f);
    void publishComment(vixac::inout::NodeId commentId, DoneFunc f);
    
    void addMember(vixac::inout::NodeId, DoneFunc);
    
    void getChatNode(NodeIdFunc) const;
    
    void getMembers(NodeIdVecFunc) const;
    void getMemberDetails(MetaVecFunc) const;
    
    void getUserIdsDarkToComment(vixac::inout::NodeId commentId, MetaMapFunc f);
    void getEggs(NodeIdVecFunc) const;
    void getEggsDetails(MetaVecFunc) const;
    
    
    void getAdmin(NodeIdFunc) const;
    void getName(StringFunc) const;
    
    void getComments(MetaMapFunc) const;
    void getLastComment(MetaMapFunc) const;

    void  phenGetName(StringFunc) const;
    
    ~Group();
private:
    void build(NodeIdFunc);
    
    void noteUpdate();
    
    void getMemberSequence(NodeIdFunc) const;
 
    
    SequenceAsync members_;
    SequenceAsync chat_;
};


class vixac::ina::GroupSpy : public vixac::ina::NodeAsyncObserver
{
public:
    GroupSpy();
    void spyOnGroup(Group group);
    void spyOnName(StringFunc);
    
    void listenOnChatThread(MetaMapFunc , bool ping = false);
    void listenOnLastComment(MetaMapFunc , bool ping = false);
    
    std::string name() const {return lastName_;}
    std::string lastComment() const {return lastComment_;}
    vixac::inout::NodeId lastAuthor() const {return lastAuthor_;}
    Group * group()  {return &group_;}
    size_t numComments()const{return numComments_;}
    
    int64_t numUnseen_;
    bool blockUnseenInc_;
private:
    
    size_t numComments_;

    vixac::inout::NodeId chatNodeId_; //only set during subscribeToNode, so not to be used outside the spy callbacks
    std::string lastName_;
    std::string lastComment_;
    vixac::inout::NodeId lastAuthor_;
    void stringChanged(StringChange  t);
    void tieGained(TieChange  t);
    
    StringFunc nameChangeFunc_;
    MetaMapFunc listenFunc_;
    MetaMapFunc lastCommentFunc_;
    Group group_;
    
    
};

#endif
