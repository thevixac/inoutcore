/*GPS.cpp
GPS
 copyright Vixac Ltd. All Rights Reserved 
*/

#include "GPS.h"
#include <iostream>
#include <string>

namespace VI = vixac;
namespace IN = VI::inout;
namespace IA = vixac::ina;

namespace
{
}

IN::GPS::GPS()
{

}

void IA::GPSAsync::timeStamp(IA::FloatFunc f) const
{
    store_->getFloatMeta(this->rootId(), "timestamp", f);
}
void IA::GPSAsync::longitude(IA::FloatFunc f) const
{
    store_->getFloatMeta(this->rootId(), "longitude", f);
    
}
void IA::GPSAsync::latitude(IA::FloatFunc f) const
{
    store_->getFloatMeta(this->rootId(), "latitude", f);
    
}
void IA::GPSAsync::degrees(IA::FloatFunc f) const
{
     store_->getFloatMeta(this->rootId(), "degrees", f);
}
void IA::GPSAsync::direction(IA::FloatFunc f) const
{
    store_->getFloatMeta(this->rootId(), "direction", f);
}
void IA::GPSAsync::speed(IA::FloatFunc f) const
{
    store_->getFloatMeta(this->rootId(), "speed", f);
}
void IA::GPSAsync::altitude(IA::FloatFunc f) const
{
    store_->getFloatMeta(this->rootId(), "altitude", f);
}
void IA::GPSAsync::setTimeStamp(double value, IA::DoneFunc f)
{
    store_->setFloatMeta(this->rootId(), "timestamp", value, f);
}
void IA::GPSAsync::setLongitude(double value, IA::DoneFunc f)
{
    store_->setFloatMeta(this->rootId(), "longitude", value, f);    
}

void IA::GPSAsync::setLatitude(double value, IA::DoneFunc f)
{
    store_->setFloatMeta(this->rootId(), "latitude", value, f);
    
}
void IA::GPSAsync::setDegrees(double value, IA::DoneFunc f)
{
    store_->setFloatMeta(this->rootId(), "degrees", value, f);
}
void IA::GPSAsync::setDirection(double value, IA::DoneFunc f)
{
    store_->setFloatMeta(this->rootId(), "direction", value, f);    
}

void IA::GPSAsync::setSpeed (double value, IA::DoneFunc f)
{
    store_->setFloatMeta(this->rootId(), "speed", value, f);
    
}
void IA::GPSAsync::setAltitude(double value, IA::DoneFunc f)
{
    store_->setFloatMeta(this->rootId(), "altitude", value, f);
    
}/*
*/