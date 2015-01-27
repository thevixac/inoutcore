/*CommentThread.h
CommentThread
 copyright Vixac Ltd. All Rights Reserved 
*/

#ifndef INCLUDED_COMMENTTHREAD
#define INCLUDED_COMMENTTHREAD


#include "Comment.h"
#include "Phenode.h"
#include "User.h"
#include "GraphUtil.h"
namespace vixac
{ 
    namespace inout
    { 
        class CommentThread;
    }
    namespace ina
    {
        class CommentThreadAsync;
    }
}

class vixac::inout::CommentThread : public vixac::inout::Phenode
{

public:
//    static CommentThread empty(vixac::inout::NodeStore * store)

    CommentThread(vixac::inout::NodeStore * store);
    CommentThread(vixac::inout::NodeId existingNodeId, vixac::inout::NodeStore * store);
    CommentThread(){}
    void addComment(std::string const& text, vixac::inout::User const& user);
    
    std::vector<vixac::inout::Comment> commentsBeforeTime(int commentCount, vixac::inout::INEpoch endTime);
private:

};

//TODO tihink this can be remvoved now that we have sequence.
class vixac::ina::CommentThreadAsync : public vixac::ina::PhenodeAsync
{
public:
    std::string phenodeType() const{ return "commentThread";}
    CommentThreadAsync(){}
    CommentThreadAsync(vixac::ina::NodeStoreAsync * store):vixac::ina::PhenodeAsync(store){}
    CommentThreadAsync(vixac::inout::NodeId nodeId, NodeStoreAsync * store): vixac::ina::PhenodeAsync(nodeId, store){}
    
    void addComment(std::string const& text, vixac::inout::NodeId userNode, DoneFunc);
    void commentsBeforeTime(int commentCount, vixac::inout::INEpoch endTime, NodeIdVecFunc);
private:
    std::vector<vixac::ina::CommentAsync> comments_;
    void build(NodeIdFunc);
};

#endif
