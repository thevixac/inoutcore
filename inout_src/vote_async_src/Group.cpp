
/*
 Group.cpp
Group
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "Group.h"
#include "Sequence.h"
#include <iostream>
#include <string>
#include "Node.h"

namespace VI = vixac;
namespace IA = VI::ina;
namespace IN = vixac::inout;



/////////////

/*
void IA::Group::phenGetEggs(IA::MetaVecFunc f) const;
{
    
}
*/
std::vector<std::string> IA::Group::getCommentFields() const
{
    std::vector<std::string> fields;
    fields.push_back("text");
    fields.push_back("author");// thank you denormalisation.
    return fields;
}
void IA::Group::noteUpdate()
{
    store_->incSequence(0, this->rootId(), IN::TieType("grp_update"), 1, true,[=]{});
}
void IA::Group::getMemberSequence(IA::NodeIdFunc f) const
{
    store_->getPrimary(this->rootId(), IN::TieType("member_sequence"),f);
}

void IA::Group::setCreator(IN::NodeId userNodeId, IA::DoneFunc f)
{
    store_->addTie(this->rootId(), IN::Tie(IN::TieType("creator"), userNodeId), f);
}
void IA::Group::setName(std::string const& name, IA::DoneFunc f)
{
    store_->setStringMeta(this->rootId(), "name", name, f);
    
    this->noteUpdate();
}

void IA::Group::getChatNode(IA::NodeIdFunc f) const
{
    store_->getPrimary(this->rootId(), IN::TieType("chat_sequence"), f);
}

void IA::Group::getMembers(IA::NodeIdVecFunc f) const
{
    this->getMemberSequence( [=](IN::NodeId sequenceNode){
        IA::SequenceAsync s( sequenceNode, store_); // I blieve this is good to use straight away?
        s.all(f);
    });
}

void IA::Group::getMemberDetails(MetaVecFunc f) const
{
    
    std::vector<IN::NodeId> seedVec;
    seedVec.push_back(this->rootId());
    std::vector<IN::TieType> walk;
    walk.push_back(IN::TieType("member_sequence"));
    walk.push_back(IN::TieType("sequence_item"));
    
    std::vector<std::string> fields;
    fields.push_back("phone_number");
    fields.push_back("acc_type");
    IA::SequenceRequest req;
    
    store_->getNqlMeta(seedVec, walk, fields, req, f);
}

//TODO test this, at least in the integration tests
void IA::Group::getEggs(IA::NodeIdVecFunc f) const
{
    abort();
    
}

void IA::Group::getEggsDetails(IA::MetaVecFunc f) const
{

    this->getMemberDetails([=](IA::MetaVec metaVec) {
        IA::MetaVec eggMetas;
        IA::MetaMap map = IA::metaVecToMap(metaVec);
        for(auto it = map.begin(), end= map.end(); it != end; ++it) {
            IA::KeyMetaMap m = it->second;
            IA::Meta& meta = m["acc_type"];
            if(meta.type_ == IA::MetaType::StringType && meta.str_ == "egg") {
                eggMetas.push_back(meta);//this works, but Id rather be less explicit and just take the whole lot. loop over keys.
                eggMetas.push_back(m["phone_number"]);//looks dangerous ey.
            }
        }
        f(eggMetas);
    });
}
void IA::Group::getAdmin(IA::NodeIdFunc f) const
{
  //TODO not sure what to do here.
}

void IA::Group::getName(IA::StringFunc f) const
{
    store_->getStringMeta(this->rootId(), "name", f);
}

void IA::Group::build(IA::NodeIdFunc f)
{
    store_->newNode(IN::NodeType("group"), [=](IN::NodeId rootNodeId){
        members_ = IA::SequenceAsync(store_);
        IA::SequenceAsync * m = &members_;
        
        chat_ = IA::SequenceAsync(store_);
        chat_.setPhenodeType("chat_sequence");
        IA::SequenceAsync * c = &chat_;
        
        members_.init([=]{
            store_->addTie(rootNodeId, IN::Tie(IN::TieType("member_sequence"), m->rootId()), [=]{

            //Theres a lot thats ugly about this code. could be parallel, could not need a private member variable, could not need a chat node at all.
                c->init([=] {
                    store_->addTie(rootNodeId, IN::Tie(IN::TieType("chat_sequence"), c->rootId()), [=] {
                        
                        f(rootNodeId);
                    });
                });
            });
        });
    });
}


void IA::Group::publishComment(IN::NodeId commentId, IA::DoneFunc f)
{
    this->getChatNode([commentId, f, this](IN::NodeId sequenceNodeId){
        this->store()->addTie(sequenceNodeId, IN::Tie(IN::TieType("comment_seqm"), commentId), f);
    });
    this->noteUpdate();
}
 

void IA::Group::addMember(IN::NodeId userNodeId, IA::DoneFunc f) {
    this->getMemberSequence([=](IN::NodeId sequenceNode) {
        IA::SequenceAsync s( sequenceNode, store_); // I blieve this is good to use straight away?
        s.addItem(userNodeId, 0, f);//TODO epoch
    });
    store_->addTie(userNodeId, IN::Tie(IN::TieType("member_of"), this->rootId()), []{});
    this->noteUpdate();
}



//TODO write a test.
void IA::Group::getComments(IA::MetaMapFunc f) const
{
    const IA::PhenodeAsync *  phen(this);
    IA::KeyMetaMap map;
    this->store()->phenGetMeta(phen, "get_comments", map,[=](IA::MetaVec v){
        IA::MetaMap m = IA::metaVecToMap(v);
        f(m);
    });
    /*
    std::vector<IN::NodeId> seedVec;
    seedVec.push_back(this->rootId());
    std::vector<IN::TieType> walk;
    walk.push_back(IN::TieType("chat_sequence"));
    walk.push_back(IN::TieType("comment_seqm"));
    
    std::vector<std::string> fields = this->getCommentFields();

    IA::SequenceRequest req;
    req.isSet_ = true; // messy API. Sequencerequest is about nodeId and tieType, which is totally ignored in this call. we only want the other 4 props.
    req.quantity = 5;
    req.asc = false;1
    req.skip =0;
    req.sort = IA::SortTypes::Creation;
    

    store_->getNqlMeta(seedVec, walk, fields,req, [=](IA::MetaVec vec) {
        f(IA::metaVecToMap(vec));
    });
     */
    
}
void IA::Group::getLastComment(IA::MetaMapFunc f) const
{
    const IA::PhenodeAsync *  phen(this);
    IA::KeyMetaMap map;
    this->store()->phenGetMeta(phen, "get_last_comment",map, [=](IA::MetaVec v){
        IA::MetaMap m = IA::metaVecToMap(v);
        f(m);
    });
}

void IA::Group::getUserIdsDarkToComment(IN::NodeId commentId, IA::MetaMapFunc f)
{
    const IA::PhenodeAsync *  phen(this);
    IA::KeyMetaMap map;
    
    std::string key ="commentId";
    IA::Meta m(IA::MetaType::IntType, key);
    m.setInt(commentId);
//    IA::Meta m(IA::MetaType::IntType, );
  //  m.setInt(commentId); //TODO shouldnt meta maybe hold a nodeId type? dunno. anyway sending nodeId as meta int is a violation.
    map[key] = m;
    this->store()->phenGetMeta(phen, "get_comment_darkness",map,[=](IA::MetaVec v){
        IA::MetaMap m = IA::metaVecToMap(v);
//        IA::Meta meta(IA::MetaType::IntType, "commentId");
        f(m);
    });
}

void IA::Group::phenGetName(IA::StringFunc f) const
{
    const IA::PhenodeAsync *  phen(this);
    IA::KeyMetaMap map;
    

    this->store()->phenGetMeta(phen, "get_name",map, [=](IA::MetaVec v){
        if(v.size() >0) {
            f(v.front().getStr());
        }
        else {
            f("");
        }
    });
    
}

IA::Group::~Group()
{
    
}

//// GroupSpy


IA::GroupSpy::GroupSpy()
: lastName_(""),
numUnseen_(0),
blockUnseenInc_(false),
nameChangeFunc_([](std::string s){std::cout<<"ERR DEFAULT spyonname CALLED " <<s<<std::endl;}),
//commentAddedFunc_([](IN::NodeId n){std::cout<<"ERR DEFAULT sypon comment CALLED" <<n<<std::endl;}),
listenFunc_([](IA::MetaMap m){std::cout<<"ERR DEFAULT sypon listenFunc_ CALLED" <<std::endl;}),
lastCommentFunc_([](IA::MetaMap m){std::cout<<"ERR DEFAULT sypon lastCommentFunc_ CALLED" <<std::endl;})
{
}

//TODO dodgy interface? kinda
void IA::GroupSpy::spyOnGroup(IA::Group group)
{
    if(group_.store())//TODO if spy moves group
    {
        std::cout<<"WARNING, spy has switched group... thats bad for some reason, i guess cos the callbacks might be all wack" <<std::endl;
//        abort();
    }
    
    group_ = group;
    group_.store()->subscribeToNode(group.rootId(), this);
    group_.getChatNode([this](IN::NodeId chatNodeId){
        chatNodeId_  = chatNodeId;
        group_.store()->subscribeToNode(chatNodeId, this);
        
    });
 
}
void IA::GroupSpy::listenOnLastComment(IA::MetaMapFunc f, bool ping)
{
    lastCommentFunc_=([=](IA::MetaMap map) {
        auto last =map.rbegin();
        if(last != map.rend()) {
            IA::KeyMetaMap keyMap = last->second;
            lastComment_ = "?";
            auto pit =keyMap.find("text");
            if(pit != keyMap.end()) {
                lastComment_ = pit->second.getStr();
            }
            auto ait = keyMap.find("author");
            lastAuthor_  = IN::Node::EMPTY_NODE_ID();
            if(ait != keyMap.end()) {
                lastAuthor_ = ait->second.getInt();
            }
        }
        f(map);
    });
    if(ping) {
        group_.getLastComment(lastCommentFunc_);
    }
}

void IA::GroupSpy::listenOnChatThread(IA::MetaMapFunc f, bool ping)
{
    listenFunc_ =([=](IA::MetaMap map) {
        f(map);
    });
    if(ping)
    {
        group_.getComments(listenFunc_);
    }
    
}

void IA::GroupSpy::spyOnName(IA::StringFunc f)
{
    
    nameChangeFunc_ = f;
    group_.phenGetName([this](std::string str){
        lastName_ = str;
        nameChangeFunc_(str);
    });
    
    group_.getName([this](std::string str) {
      //  lastName_ = str;
       // nameChangeFunc_(str);
    });
    
}

void IA::GroupSpy::tieGained(IA::TieChange change)
{
    if(change.nodeId == chatNodeId_ && change.tie.type().type == "comment_seqm")
    {
        std::vector<IN::NodeId> seedVec;
        seedVec.push_back(change.tie.target());
        std::vector<std::string> fields = group_.getCommentFields();
        std::vector<IN::TieType> noWalk;
        IA::SequenceRequest noReq;
        //TODO this works, but its a double hit. TODO subscribe directly to nql.
        group_.store()->getNqlMeta(seedVec, noWalk, fields,noReq, [=](IA::MetaVec vec) {

            if(!blockUnseenInc_) {
                numUnseen_++;
            }
            std::cout<<"num unseen is now " <<numUnseen_ <<std::endl;
            listenFunc_(IA::metaVecToMap(vec));
            lastCommentFunc_(IA::metaVecToMap(vec));
        });
    }
}

void IA::GroupSpy::stringChanged(IA::StringChange  change)
{
    if(change.key == "name")
    {
        lastName_ = change.value;
        nameChangeFunc_(lastName_);
    }
}