/*PhenodeAsync.h
PhenodeAsync
 copyright Vixac Ltd. All Rights Reserved 
*/

#ifndef INCLUDED_PHENODEASYNC
#define INCLUDED_PHENODEASYNC


#include <string>
#include "Node.h"
#include "AsyncFunctor.h"
#include "NodeStoreAsync.h"
#include "Node.h"
#include <iostream>
namespace vixac
{ 
    namespace ina
    { 
        class PhenodeAsync;
        
    }
}
namespace
{
    int t_count=0;
}

class vixac::ina::PhenodeAsync
{

public: 

    PhenodeAsync():store_(NULL), rootId_(vixac::inout::Node::EMPTY_NODE_ID()), isValid_(false){}//isValid should be false
    PhenodeAsync(vixac::ina::NodeStoreAsync * store):store_(store) ,rootId_(vixac::inout::Node::EMPTY_NODE_ID()){}
    PhenodeAsync(vixac::inout::NodeId nodeId, vixac::ina::NodeStoreAsync * store):rootId_(nodeId), store_(store){}
    
    

    virtual std::string phenodeType() const{ return ".";}//TODO seems forgettable to subclass.
    vixac::inout::NodeId rootId() const {return rootId_;}
//    void root(NodeFunc  )const;//TODO im thinking. either return a copy, or just never call this.
    //hopefully we can break down the usage to queries to the store.
    
    void init(DoneFunc f);
    
    PhenodeAsync(PhenodeAsync const& rhs) {
       // std::cout<<"COPY phenode" << ++::t_count<< this->phenodeType() <<", " <<rootId_<<std::endl;

        this->isValid_ = rhs.isValid_;
        this->rootId_ = rhs.rootId_;
        this->store_ = rhs.store_;
    }
    vixac::ina::NodeStoreAsync * store()const{return store_;}
protected:
    
    //TODO heres all the code that constructs. always call store_->newNode i think.
    virtual void build(NodeIdFunc f);
    //TODO currently the phenode does not hang on to the pointer to its own shiz. thats an optimisation
    //TODO maybe we can move away from a pointer based api entirely? you getNode copies? fuck that.. ?
    // all that copying. let the store just garuntee it.

    bool isValid_;
    vixac::inout::NodeId rootId_;
    vixac::ina::NodeStoreAsync * store_;
    


};

#endif
