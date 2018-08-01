#ifndef _STD_THREAD_BARRIER
#define _STD_THREAD_BARRIER

#include <condition_variable>
#include <mutex>

/*!
    \brief barrier Synchronisierung für std::threads

    Die Funktion wait() kann zum Synchronisieren z.B. am Ende einer Zeile aufgerufen werden.
    In der C++ Standard Library ist kein eigenes barrier vorhanden, 
    jedoch basiert die Implementierung von StdThreadBarrier auf Mitteln aus den Headern:
    <condition_variable> und <mutex> der std.
 */
class StdThreadBarrier {
public:
    StdThreadBarrier( int );

    void wait();
private:
    const int _threadCount;
    int _callCount, _barrierGroup;

    std::mutex _mutex;
    std::condition_variable _condVar;
};

#endif