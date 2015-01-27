/*GPS.h
GPS
 copyright Vixac Ltd. All Rights Reserved 
*/

#ifndef INCLUDED_GPS
#define INCLUDED_GPS


#include "PhenodeAsync.h"
#include "Phenode.h"
namespace vixac
{ 
    namespace inout
    { 
        class GPS;
    }
    namespace ina
    
    {
        class GPSAsync;
    }
}

class vixac::inout::GPS
{

public: 

    GPS();
private: 

};


class vixac::ina::GPSAsync : public vixac::ina::PhenodeAsync
{
public:
    std::string phenodeType() const{ return "gps";}
    GPSAsync(){}
    GPSAsync(NodeStoreAsync * store): vixac::ina::PhenodeAsync(store){}
    GPSAsync(vixac::inout::NodeId nodeId, NodeStoreAsync * store): vixac::ina::PhenodeAsync(nodeId, store){}
    
    void timeStamp(FloatFunc) const;
    void longitude(FloatFunc) const;
    void latitude(FloatFunc) const;
    void degrees(FloatFunc) const;
    void direction(FloatFunc) const;
    void speed(FloatFunc) const;
    void altitude(FloatFunc) const;
    
    void setTimeStamp(double value, DoneFunc);
    void setLongitude(double value, DoneFunc);
    void setLatitude(double value, DoneFunc);
    void setDegrees(double value, DoneFunc);
    void setDirection(double value, DoneFunc);
    void setSpeed(double value, DoneFunc);
    void setAltitude(double value, DoneFunc);
    
    
};
#endif
