/*Wish.cpp
Wish
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "Wish.h"
#include <iostream>
#include <string>
#include "UserWish.h"
namespace VI = vixac;
namespace IN = VI::inout;
namespace IA = vixac::ina;


void IA::Wish::getJobs(IA::NodeStoreAsync * store,IN::NodeId userNodeId, IA::MetaMapFunc f)
{
    IA::UserWish user(userNodeId, store);
    IA::KeyMetaMap map;
    store->phenGetMeta(&user, "get_jobs", map, [=](IA::MetaVec metaVec){
        IA::MetaMap map = IA::metaVecToMap(metaVec);
        f(map);
    });
}


void IA::Wish::getOutsourced(IA::NodeStoreAsync * store,IN::NodeId userNodeId, IA::MetaMapFunc f)
{
    IA::UserWish user(userNodeId, store);
    IA::KeyMetaMap map;
    store->phenGetMeta(&user, "get_outsourced", map, [=](IA::MetaVec metaVec){
        IA::MetaMap map = IA::metaVecToMap(metaVec);
        f(map);
    });
}

void IA::Wish::createWish(NodeStoreAsync * store, IN::NodeId creator, IA::BoolFunc f)
{
    IA::UserWish user(creator, store);
    IA::KeyMetaMap map;
    store->phenDo(&user, "create_wish", map, f);
}

void IA::Wish::getSummary(IA::NodeStoreAsync * store, IN::NodeId wishId, IA::StringFunc f)
{
    store->getStringMeta(wishId, "summary", f);
}