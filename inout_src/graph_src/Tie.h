/*Tie.h
Tie
 copyright Vixac Ltd. All Rights Reserved 
*/

#ifndef INCLUDED_TIE
#define INCLUDED_TIE

#include "GraphUtil.h"

#include <string>
#include <iostream>
namespace vixac
{ 
    namespace inout
    { 
        class Tie;
        struct TieType
        {
            TieType(){}
            TieType(std::string const& type):type(type){}
            std::string type;

            std::string typeConst() const {return type;}
            bool operator<(vixac::inout::TieType const& rhs) const
            {
                return this->type < rhs.typeConst();
            }
        };
        struct NodeType
        {
            NodeType(){}
            NodeType(std::string const& type):type(type){}
            std::string type;
            
            std::string typeConst() const {return type;}
            bool operator<(vixac::inout::TieType const& rhs) const
            {
                return this->type < rhs.typeConst();
            }
        };
    }
}


//TODO rm this class i think. there is only the tieType, which is a whole kettle of fish
class vixac::inout::Tie
{

public: 

    Tie(){}
    explicit Tie(TieType const& type, vixac::inout::NodeId nodeId, vixac::inout::INEpoch stamp=0) :type_(type), nodeId_(nodeId), stamp_(stamp), deleted_(false), seq_(0){}
    TieType type() const {return type_;}
    vixac::inout::NodeId target() const { return nodeId_;}
    vixac::inout::INEpoch epoch() const { return stamp_;}
    
    int64_t getSeq() const{return seq_;}
    void setSeq(int64_t seq) {seq_ = seq;}
    void incSeq(int64_t seq) {
        seq_+=seq;
    }
    bool deleted() const {return deleted_;}
    
    //TODO probably dont want to call this directly has you probably have a copy of tie, so you wont be messing with the state of store, use node->flagTieForDeletion.
    void flagForDeletion(){ deleted_ = true;}
    //TODO test that ties get ordered properly
    bool operator<(vixac::inout::Tie const& rhs){ return this->stamp_ < rhs.epoch();}
private:
    int64_t seq_;
    bool deleted_;
    vixac::inout::INEpoch stamp_;
    vixac::inout::NodeId nodeId_;
    
    TieType type_;

};

#endif
