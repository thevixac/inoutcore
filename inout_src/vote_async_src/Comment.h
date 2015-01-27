/*Comment.h
Comment
 copyright Vixac Ltd. All Rights Reserved 
*/

#ifndef INCLUDED_COMMENT
#define INCLUDED_COMMENT

#include "Phenode.h"
#include "Node.h"
#include "NodeStore.h"
#include "GraphUtil.h"

#include "PhenodeAsync.h"
#include "UserAsync.h"
#include <iostream>

namespace vixac
{ 
    namespace inout
    { 
        class Comment;
        //--//
        class User;

    }
    namespace ina
    {
        class CommentAsync;
        class UserAsync;
        class CommentSpy;
    }
}

class vixac::inout::Comment : public vixac::inout::Phenode
{

public:
        std::string phenodeType() const{return "comment";};
    static Comment empty(vixac::inout::NodeStore * store);

    bool isValid() const {return false;}
    Comment(vixac::inout::NodeStore * store, vixac::inout::User const& user, std::string const& text ="");
    Comment(vixac::inout::NodeId existingId, vixac::inout::NodeStore * store);
    Comment(){}
    
    
    void setText();

    std::string text() const;
    vixac::inout::Node * getUserNode() const;
//    vixac::inout::Node * getTargetNode() const;
//    vixac::inout::INEpoch epoch() const;
    
private:
//    vixac::inout::NodeId userNodeId_;

};


class vixac::ina::CommentAsync : public vixac::ina::PhenodeAsync
{
public:
    std::string phenodeType() const{ return "comment";}
    CommentAsync(){}
    CommentAsync(NodeStoreAsync * store): vixac::ina::PhenodeAsync(store){}
    CommentAsync(vixac::inout::NodeId nodeId, NodeStoreAsync * store): vixac::ina::PhenodeAsync(nodeId, store){}
    
    
    //TODO this is crazy confusing. one means create new
    CommentAsync(NodeStoreAsync * store,
                 vixac::inout::NodeId userRoot, std::string const& str)
    : vixac::ina::PhenodeAsync( store),
    userRoot_(userRoot),
    str_(str)
    {}
    

    void setText(std::string const&, DoneFunc);
    void text(StringFunc);
    void author(NodeIdFunc);
    
    ~CommentAsync(){}
    
private:
    void build(NodeIdFunc);
    vixac::inout::NodeId userRoot_;
    std::string str_;
};


class vixac::ina::CommentSpy : public vixac::ina::NodeAsyncObserver
{
public:
    CommentSpy();
    void spyOnComment(CommentAsync comment);
    void spyOnText(StringFunc f);//TODO sort this out
    void spyOnAuthorNumber(StringFunc f);
 
    std::string  text(){return text_;};
    std::string  authorNum(){return authorNum_;}
    vixac::inout::NodeId authorId(){return authorId_;}
private:
    void stringChanged(StringChange t);

    std::string text_;
    std::string authorNum_;
    vixac::inout::NodeId authorId_;
    vixac::inout::NodeId commentId_;
    StringFunc textFunc_;
    StringFunc authorFunc_;
    CommentAsync comment_;
    UserAsync author_;

    
    
};

#endif
