/*NodeAsyncTest.cpp
NodeAsync
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "NodeAsync.h"

#include "LocalNodeStore.h"
#include <gtest/gtest.h>



namespace VI = vixac;
namespace IA = VI::ina;
namespace IN = vixac::inout;

/**
 void addTie(vixac::inout::TieType type, vixac::inout::NodeId targetId,  DoneFunc f, bool notifyTarget=true);
 void setStringMeta(std::string const& key, std::string const& value, DoneFunc f);
 void setIntMeta(std::string const& key, int64_t value, DoneFunc f);
 void getStringMeta(std::string const& key, StringFunc f);
 void getIntMeta(std::string const& key, IntFunc f);
 void getPrimary(vixac::inout::TieType type, NodeIdFunc f);
 void getActiveTies(vixac::inout::TieType type, NodeIdVecFunc f);

*/

TEST(NodeAsyncTest, setStringMeta)
{

}