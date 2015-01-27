/*CommentThread.cpp
CommentThread
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "CommentThread.h"
#include <iostream>
#include <string>

namespace VI = vixac;
namespace IN = VI::inout;
namespace IA = vixac::ina;


IN::CommentThread::CommentThread(IN::NodeStore * store)
:IN::Phenode(store->newNode(), store)
{}

IN::CommentThread::CommentThread(IN::NodeId existingId, IN::NodeStore * store)
: IN::Phenode(store->getNode(existingId), store)
{}

void IN::CommentThread::addComment(std::string const& text, IN::User const& user)
{
    IN::Comment comment(store_, user, text);
    IN::TieType commentType("comment");
    store_->addTie(root_->nodeId(), IN::Tie(commentType, comment.root()->nodeId(), store_->now()));
}

//TODO not using comment count or enttime yet.
std::vector<IN::Comment> IN::CommentThread::commentsBeforeTime(int commentCount, IN::INEpoch endTime)
{
    std::vector<IN::Comment> comments;
    IN::TieType titleType("comment");
    IN::TieTypeIteratorConst it = root_->getTies(titleType);
    if(it == root_->tieIteratorEnd())
    {
        std::cout<<"no comments found" <<std::endl;
    }
    std::vector<IN::Tie> const& ties = it->second;
    for(int i=0; i< ties.size(); i++)
    {
        comments.push_back(IN::Comment(ties[i].target(), store_));
    }
    return comments;
}

//// async time

void IA::CommentThreadAsync::addComment(std::string const& text, vixac::inout::NodeId userNode, IA::DoneFunc f)
{
    comments_.push_back(IA::CommentAsync(store_, userNode, text));//this needs to be stored someplace.
    size_t commentLocation = comments_.size()-1; //TODO this is a tiny race with the line above, in case of duplicate comments added

    comments_.back().init([=](){
        IN::NodeId commentRootId = comments_[commentLocation].rootId();//TODO this is a race.
        store_->addTie(this->rootId(), IN::Tie(IN::TieType("comment"), commentRootId), f);
    });
}
void IA::CommentThreadAsync::commentsBeforeTime(int commentCount, vixac::inout::INEpoch endTime, IA::NodeIdVecFunc f)
{
    //TODO how about the before time bit.... store_ needs to support that
    store_->getActiveTies(this->rootId(), IN::TieType("comment"), f);
}

void IA::CommentThreadAsync::build(NodeIdFunc f)
{
    store_->newNode(IN::NodeType("comment_thread"), f);
}