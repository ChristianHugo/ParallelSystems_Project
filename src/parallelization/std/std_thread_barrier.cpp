#include "parallelization/std/std_thread_barrier.h"

#include <thread>

using namespace std;


/*!
    \param erzeugt ein StdThreadBarrier Objekt für "threadCount" Objekte

    \param threadCount Anzahl der wartenden Threads
 */
StdThreadBarrier::StdThreadBarrier( int threadCount ) :
    _threadCount( threadCount ),
    _callCount( 0 ),
    _barrierGroup( 0 )
{}


/*!
    \brief lässt einen Thread warten
    
    Der aufrufende Thread wchselt solange in den WAIT Status, bis "threadCount" Threads
    wait() aufgerufen haben
 */

 
void StdThreadBarrier::wait() 
{
    unique_lock< mutex > lock( _mutex );
    int curBarrierGroup = _barrierGroup;    
    // von allen threads aufgerufen ?
    if ( ++_callCount == _threadCount )
    {
        _callCount = 0;
        _barrierGroup++;
        // Versuch alle aufzuwecken
        _condVar.notify_all();
    }
    else
    {        
        _condVar.wait( lock, [ this, curBarrierGroup ] 
        { 
            // threads nicht in "curBarrierGroup" warten 
            return curBarrierGroup == _barrierGroup - 1; 
        });
    }
}