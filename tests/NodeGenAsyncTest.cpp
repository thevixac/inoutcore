/*NodeGenAsyncTest.cpp
NodeGenAsync
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "NodeGenAsync.h"

#include <gtest/gtest.h>

#include "Node.h"
#include "AsyncFunctor.h"

namespace VI = vixac;
namespace IA = VI::ina;
namespace IN = vixac::inout;

namespace
{
    struct SimpleGenFunc
    {
        SimpleGenFunc():count_(0){}
        
        
        //void operator()(std::vector<vixac::inout::NodeId> ids)
        void incCount(std::vector<IN::NodeId> ids)
        {
            count_+= ids.size();
        }
        size_t count_;
    };
    
    class SimpleAsyncGen : public IA::NodeGenAsync
    {
    public:
        SimpleAsyncGen():count_(0){}
        void a_get(size_t quantity, IA::NodeIdVecFunc  f)
        {
            std::vector<IN::NodeId> nodeIds;
            
            size_t last = count_+ quantity;
            
            for(size_t i=count_; i < last; i++ )
            {
                nodeIds.push_back(i);
            }
            count_ =last;
            (f)(nodeIds);
        }
    private:
        size_t count_;
    };
    
    
}
TEST(NodeIdReplaceFunctor, Async)
{
    ::SimpleGenFunc f;
    ::SimpleAsyncGen g;
    g.a_get(10,  std::bind(&::SimpleGenFunc::incCount, &f, std::placeholders::_1));
    
    //its in that callback that f can be destroyed. remove it from a container or something.
    EXPECT_EQ(f.count_, 10); //now to worry aboutt he lifetime of this functor.
    // in the async world it needs to die when its done. so the generator should own it.
}


