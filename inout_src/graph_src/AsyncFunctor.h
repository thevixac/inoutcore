//
//  AsyncFunctor.h
//  inoutcore
//
//  Created by vic on 07/03/2014.
//  Copyright (c) 2014 vixac. All rights reserved.
//

#ifndef inoutcore_AsyncFunctor_h
#define inoutcore_AsyncFunctor_h


#include <algorithm>

#include <memory>
#include <memory.h>
#include "Node.h" //TODO get rid of this for sure.
namespace vixac
{


    //--//
    namespace ina
    {
        class NodeAsync;
        
        
        namespace MetaType //TODO make same as metatypes
        {
            enum Type
            {

                IntType,
                StringType,
                FloatType,
            };
        }
        
        
        struct Meta //a single meta datum. a key, a value, a type, and a nodeId path to it.
        {
            Meta(){}//for stl
            Meta(std::vector<vixac::inout::NodeId> const& nodeIds, vixac::ina::MetaType::Type type, std::string const& key):nodeIds_(nodeIds),type_(type), key_(key){}
            Meta(vixac::ina::MetaType::Type type, std::string const& key):type_(type), key_(key){}
            void setString(std::string const& value){str_ =value;}
            void setInt(int64_t value){int_ =value;}
            void setFloat(float value){double_ =value;}
            
            std::string getStr()const{return str_;}
            int64_t getInt()const{return int_;}
            double getFloat()const{return double_;}
            vixac::inout::NodeId rootId()const{ return nodeIds_.back();}
            std::vector<vixac::inout::NodeId> walk()const {return nodeIds_;}
            
            std::vector<vixac::inout::NodeId> nodeIds_; //this is a path, not a bunch of nodeIds.
            vixac::ina::MetaType::Type type_;
            
            std::string key_;
            std::string str_;
            int64_t int_;
            double double_;
        };


        typedef std::vector<vixac::ina::Meta> MetaVec; // every meta node,key,value,type object in no order
        typedef std::map<std::string,vixac::ina::Meta> KeyMetaMap; //you can look up metas by key. thats awesome
        typedef std::map<vixac::inout::NodeId, KeyMetaMap> MetaMap; //you can get all metas for a given nodeId.
        typedef std::vector<vixac::inout::NodeId> NodeIdVec;
        
        
        

        typedef std::function<void (vixac::ina::NodeAsync )> NodeFunc; //TODO rm i think, or keep personal to the store
        typedef std::function<void (vixac::inout::Node * )> NodePtrFunc; 
        typedef std::function<void (void)> DoneFunc;
        typedef std::function<void (bool)> BoolFunc;
        typedef std::function<void (vixac::inout::NodeId)> NodeIdFunc;
        typedef std::function<void (int64_t)> IntFunc;
        typedef std::function<void (double)> FloatFunc;
        
        typedef std::function<void (std::string)> StringFunc;
        typedef std::function<void (vixac::ina::NodeIdVec)> NodeIdVecFunc;
        
        typedef std::function<void  (vixac::ina::MetaVec)> MetaVecFunc;
        typedef std::function<void  (vixac::ina::MetaMap)> MetaMapFunc;
        

        vixac::ina::MetaMap metaVecToMap(vixac::ina::MetaVec const& vec);

        
         struct TieChange{

             TieChange():nodeId(vixac::inout::Node::EMPTY_NODE_ID()){}
             TieChange(vixac::inout::NodeId n, vixac::inout::Tie t) :nodeId(n), tie(t){}
             vixac::inout::NodeId nodeId;
             vixac::inout::Tie tie;
         };
         struct StringChange {
             StringChange():nodeId(vixac::inout::Node::EMPTY_NODE_ID()){}
        
             StringChange(vixac::inout::NodeId n, std::string k, std::string v):nodeId(n),key(k), value(v){}
         vixac::inout::NodeId nodeId;
         std::string key;
         std::string value;
         };
         struct IntChange {
             IntChange():nodeId(vixac::inout::Node::EMPTY_NODE_ID()){}
         IntChange(vixac::inout::NodeId n, std::string k, int64_t v):key(k), value(v){}
         vixac::inout::NodeId nodeId;
         std::string key;
         int64_t value;
         };
        
        struct FloatChange {
            FloatChange():nodeId(vixac::inout::Node::EMPTY_NODE_ID()){}
            FloatChange(vixac::inout::NodeId n, std::string k, double v):key(k), value(v){}
            vixac::inout::NodeId nodeId;
            std::string key;
            double value;
        };
         struct NodeIdChange {
            NodeIdChange() :nodeId(vixac::inout::Node::EMPTY_NODE_ID()), oldNodeId(vixac::inout::Node::EMPTY_NODE_ID()){}
             
         NodeIdChange(vixac::inout::NodeId n, vixac::inout::NodeId o): nodeId(n), oldNodeId(o){}
         vixac::inout::NodeId nodeId;
         vixac::inout::NodeId oldNodeId;
         };

        typedef std::function<void (TieChange)> TieChangeFunc;
        typedef std::function<void (StringChange)> StringChangeFunc;
        typedef std::function<void (IntChange)> IntChangeFunc;
        typedef std::function<void (FloatChange)> FloatChangeFunc;
        typedef std::function<void (NodeIdChange)>NodeIdChangeFunc;
    }
}

#endif
