#ifndef _STD_THREADS_STRATEGY
#define _STD_THREADS_STRATEGY

#include "parallelization/std/std_thread_barrier.h"
#include "parallelization/strategy/strategy_base.h"
#include "processing/wave_processing.h"

#include <memory>
#include <string>

class StdThreadBarrier;
class WaveProcessingParams;


/*!
    \brief mit std::threads parallelisierte Strategie

    Leitet von StrategyBase ab und parallelisiert das Approximieren 
    mit threads aus der C++ Standard Library.
 */
class StdThreadsStrategy : public StrategyBase {
public:
    StdThreadsStrategy( const WaveProcessingParams & );
    
    virtual double * operator()() override;
    virtual std::string getStrategyName() override;
    
private:
    void processPartial( int, int, double * );
    StdThreadBarrier _barrier;
    WavePocessing _proc;
};

#endif