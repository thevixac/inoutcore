/*VXObserver.h
VXObserver
 copyright Vixac Ltd. All Rights Reserved 
*/

#ifndef INCLUDED_VXOBSERVER
#define INCLUDED_VXOBSERVER

#include <vector>

namespace vixac
{ 
    namespace inout
    {
        class VXObservable;
        class VXObserver;
        class VXObservedChange;
    }
}


/**
  Im not using this because the actual function  that gets called back
  may want any parameter list, which is a bitch to code up.
 its doable though: http://stackoverflow.com/questions/9568150/what-is-a-c-delegate
  in the meantime, im just gna xpaste the very simple observer code into node.
 
 */

class vixac::inout::VXObserver
{

public:
private: 

};

/**
  does not handle anything other than simply storing the observers.
  the sending to the observers has to be done by the subclass of vxobservable via
  some notifyObservers(change) { for_each(observer_ change())} or whatevs
 */
class vixac::inout::VXObservable
{
    
public:
    void addObserver(vixac::inout::VXObserver * observer);
    void removeObserver(vixac::inout::VXObserver  * observer);
    
protected:
   // virtual void notifyObservers() =0; // simply do for_each(observers_, something);

    std::vector<vixac::inout::VXObserver * > observers_;
    
};

#endif
