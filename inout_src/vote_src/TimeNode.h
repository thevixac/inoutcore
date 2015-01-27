/*TimeNode.h
TimeNode
 copyright Vixac Ltd. All Rights Reserved 
*/

#ifndef INCLUDED_TIMENODE
#define INCLUDED_TIMENODE

#include "Phenode.h"
#include "NodeStore.h"
namespace vixac
{
    namespace inout
    {
        class TimeNode;
        namespace TimeResolution
        {
            enum Type
            {
                Day,
                Hour,
                Minute,
                Second,
                LastDateResolution
            };
        }
        namespace DayOfWeek
        {
            enum Type
            {
                Mon,
                Tue,
                Wed,
                Thur,
                Fri,
                Sat,
                Sun
            };
        };
        
        const int64_t SEC_MINUTE=60;
        const int64_t SEC_HOUR = SEC_MINUTE*60;
        const int64_t SEC_DAY = SEC_HOUR*24;
        const int64_t SEC_WEEK = SEC_DAY*7;
        
    }
}

class vixac::inout::TimeNode : public vixac::inout::Phenode

{
    
public:
    TimeNode(vixac::inout::NodeStore * store, vixac::inout::INEpoch, vixac::inout::TimeResolution::Type,  int64_t durationInSeconds =0);
    TimeNode(vixac::inout::NodeId existing, vixac::inout::NodeStore * store);
    
    vixac::inout::INEpoch epoch() const;
    void setEpoch(vixac::inout::INEpoch) const;
    
    int64_t durationInSeconds() const;
    vixac::inout::TimeResolution::Type resolution() const; // maybe we dont need this.
private:
    
    
};

#endif
