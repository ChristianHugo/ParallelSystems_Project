#ifndef _STRATEGY_TESTER
#define _STRATEGY_TESTER

#include "processing/wave_processing_params.h"
#include "parallelization/strategy/strategy_context.h"
#include "parallelization/strategy/strategy_base.h"
#include "parallelization/strategy/strategy_type.h"
#include "parallelization/std/std_threads_strategy.h"


/*!

 */
class StrategyTester
{
public:
    StrategyTester( int, int, int );
    ~StrategyTester();

    double *executeWaveProcessing( STRATEGY_TYPE );


    void checkMatrix( double * );

    void dumpRunStats();

    void resetMatrix( int, int );
    void resetNumThreads( int );
private:
    StrategyContext _ctx;

    int _rows, _columns;
    int _numThreads;
    double *_expectedData;
private:
    void logExecuteStrategy( StrategyBase::ptr & );

    double getAt( int, int, double * );
};

#endif