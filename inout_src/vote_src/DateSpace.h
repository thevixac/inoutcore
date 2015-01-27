/*
 DateSpace.h
 DateSpace
 copyright Vixac Ltd. All Rights Reserved 
*/

#ifndef INCLUDED_DATESPACE
#define INCLUDED_DATESPACE

#include <stdint.h>
namespace vixac
{ 
    namespace inout
    { 
        class DateSpace;
        
        namespace DateResolution
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
    }
}

/**
 //TODO delete this i guess.
 Not sure what im going to need exactly here
 definitely greate
 */

class vixac::inout::DateSpace
{

public: 

    DateSpace();
    void setStartEpoch(int64_t start, vixac::inout::DateResolution::Type resolution);
    void setDuration(int64_t seconds);
    bool hasDuration() const { return duration_ > 0;}
    vixac::inout::DateResolution::Type resolution() const { return resolution_;}
    bool operator<(vixac::inout::DateSpace const& rhs);
    
    int64_t start() const {return startEpoch_;}
private:
    vixac::inout::DateResolution::Type resolution_;
    int64_t startEpoch_;
    int64_t duration_;
};

#endif
