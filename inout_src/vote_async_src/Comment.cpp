/*Comment.cpp
Comment
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "Comment.h"
#include "User.h"

#include <iostream>
#include <string>

namespace VI = vixac;
namespace IN = VI::inout;
namespace IA = vixac::ina;


IN::Comment IN::Comment::empty(IN::NodeStore * store)
{
    IN::Comment comment(store, IN::User(store), "");
    return comment;
}


IN::Comment::Comment(IN::NodeStore * store, IN::User const& user, std::string const& text)
:IN::Phenode(store->newNode(), store)
{
    root_->setString("text", text);
    root_->setInt("author", user.root()->nodeId()); // this is flattening. its a dirty trick to shorten a walk.
    IN::TieType authorType("author");
    store_->addTie(root_->nodeId(), IN::Tie(authorType, user.root()->nodeId(), store_->now()));
}

IN::Comment::Comment(IN::NodeId nodeId, IN::NodeStore * store) :
IN::Phenode(store->getNode(nodeId), store)
{
    // this is from an existing node,
}

void IN::Comment::setText()
{
    std::cout<<"TODO this is always an edit" <<std::endl;
}

std::string IN::Comment::text() const
{
    return root_->getString("text");
}

IN::Node * IN::Comment::getUserNode() const
{
    return store_->getNode(this->root()->getPrimary(IN::TieType("author")));
}



//////////////////


///////// Comment Async






void IA::CommentAsync::setText(std::string const& str, IA::DoneFunc f)
{
    std::cout<<"TODO allow edits on comments: " <<str <<std::endl;
    f();
}
void IA::CommentAsync::text(IA::StringFunc f)
{
    store_->getStringMeta(this->rootId(), "text", f);
}


void IA::CommentAsync::author(IA::NodeIdFunc f)
{
    store_->getPrimary(this->rootId(), IN::TieType("author"), f);
}

void IA::CommentAsync::build(IA::NodeIdFunc f)
{
    //TODO VXOPT parallelise. We dont need everything to be complete to return.

    store_->newNode(IN::NodeType("comment"),[this,f](IN::NodeId nodeId){
        store_->setStringMeta(nodeId, "text", str_, [this,nodeId,f](){
            store_->addTie(nodeId, IN::Tie(IN::TieType("author"), userRoot_ ), [=]() {
                store_->setIntMeta(nodeId, "author", userRoot_, [=](){ //TODO thats denormalising.
                    f(nodeId);
                });
                
            });
        });
    });
}



//////// commentSpy

IA::CommentSpy::CommentSpy()
:textFunc_([](std::string){}),
authorFunc_([](std::string){})
{}


void IA::CommentSpy::spyOnComment(IA::CommentAsync comment)
{
    comment_ = comment;
    comment_.store()->subscribeToNode(comment_.rootId(), this);//TODO can unspy just by not calling this. consider refactoring
    comment_.author([this](IN::NodeId authorId) {
        authorId_ = authorId;
        author_ = IA::UserAsync(authorId_, comment_.store());
        comment_.store()->subscribeToNode(authorId, this);
        author_.getMobile([this](std::string str) {
            authorNum_ = str;
            authorFunc_(str);
        });
    });
    
}
void IA::CommentSpy::spyOnText(IA::StringFunc f)
{
    textFunc_= f;
    comment_.text([this](std::string str){
        text_ =str;
        textFunc_(str);
    });
}

//TODO rm.
void IA::CommentSpy::spyOnAuthorNumber(IA::StringFunc f)
{
    authorFunc_ = f;
}

void IA::CommentSpy::stringChanged(IA::StringChange t)
{
    if(t.nodeId == authorId_)
    {
        if(t.key == "phone_number")
        {
            authorNum_ = t.value;
            authorFunc_(t.value);
        }
    }
    else if(t.nodeId == commentId_)
    {
        if(t.key == "text")
        {
            text_ = t.value;
            textFunc_(t.value);
        }
    }
}