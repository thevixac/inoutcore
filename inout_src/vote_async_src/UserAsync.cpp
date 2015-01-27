/*UserAsync.cpp
UserAsync
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "UserAsync.h"
#include <iostream>
#include <string>

namespace VI = vixac;
namespace IA = VI::ina;
namespace IN = vixac::inout;

void IA::UserAsync::build(IA::NodeIdFunc f)
{

    if(this->rootId()!= IN::Node::EMPTY_NODE_ID()) {
        abort(); //must not call build when building from an existing constructor
    }
    
    IA::NodeStoreAsync * localStorePtr = store_;
    store_->newNode(IN::NodeType("user"),[=](IN::NodeId rootNodeId) {
        //this->rootId_ = rootNodeId ; //SHORTCUT
        IA::NodeStoreAsync * s = localStorePtr;
        s->newNode(IN::NodeType("name"),[=](IN::NodeId secondNodeId){ //TODO fuck this amirite. just shove name in the meta
            store_->addTie(rootNodeId, IN::Tie(IN::TieType("name"), secondNodeId), [=](){
                if(str_ !="")
                {
                    std::cout<<"so root here is:: TODO put this back " <<this->rootId()<<std::endl;
                 //   this->setName(str_, [=]{ //TODO doesnt work because rootNodeId isnt set.
                        f(rootNodeId);
                   // });
                }
                else
                {
                    f(rootNodeId);
                }                
            });
        });
    });
    
}

void IA::UserAsync::getGroupDetail(IA::MetaMapFunc f) {
    IA::PhenodeAsync * phen = this;
    IA::KeyMetaMap map;
    store_->phenGetMeta(phen,"get_groups_detail", map,[=](IA::MetaVec v){
        IA::MetaMap m = IA::metaVecToMap(v);
        f(m);
    });
    
}

void IA::UserAsync::name(IA::StringFunc f) const
{
    if(this->rootId() == IN::Node::EMPTY_NODE_ID()) {
        abort();
    }
    store_->getPrimary(this->rootId(), IN::TieType("name"), [=](IN::NodeId nodeId){
        store_->getStringMeta(nodeId  , "name", f);
    });
}

void IA::UserAsync::setName(const std::string &str, IA::DoneFunc f) const
{
    this->setName(str, this->rootId(), f);
}

void IA::UserAsync::setName(std::string const& str, IN::NodeId rootNodeId, IA::DoneFunc  f) const
{
    if(rootNodeId == IN::Node::EMPTY_NODE_ID()) {
        abort();
    }
    IN::TieType type = IN::TieType("name");
    IN::NodeId theRoot = rootNodeId;
    store_->getPrimary(theRoot, type, [this,theRoot, f,str](IN::NodeId nodeId){
        store_->setStringMeta(nodeId  , "name", str, f);
    });
}

void IA::UserAsync::getMobile(IA::StringFunc f)
{
    std::cout<<"getting mobile for root " <<this->rootId()<<std::endl;
    store_->getStringMeta(this->rootId(), "phone_number", f);
}

void IA::UserAsync::nameNode(IA::NodeIdFunc f)
{
    store_->getPrimary(this->rootId(), IN::TieType("name"), f);
}
void IA::UserAsync::setScreen(std::string const& str, IA::DoneFunc f) const
{
    
    IA::KeyMetaMap data;
    //vixac::ina::MetaType::Type type, std::string const& key):nodeIds_(nodeIds),type_(type), key_(key){}
    std::string key ="screen";
    IA::Meta m(IA::MetaType::StringType, key);
    m.setString(str);
    data[key] = m;
    store_->phenDo(this, "set_cur_screen", data, [=](bool ok){
        f();
    });
     
 //   store_->setStringMeta(this->rootId(), "cur_screen", str, f);
}

void IA::UserAsync::getGroups(IA::NodeIdVecFunc f)
{
    IA::SequenceRequest request(this->rootId(), IN::TieType("member_of"));
    request.asc = false;
    request.sort= IA::SortTypes::Update;
    store_->getSequence(request, f);
}