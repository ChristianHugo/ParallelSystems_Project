#ifndef _SEQUENTIAL_STRATEGY
#define _SEQUENTIAL_STRATEGY

#include "parallelization/strategy/strategy_base.h"
#include "processing/wave_processing.h"

#include <string>

class WaveProcessingParams;


/*!
    \brief Strategie ohne Parallelisierung

    Leitet von StrategyBase ab und approximiert die Wellengleichung sequentiell. 
 */
class SequentialStrategy : public StrategyBase
{
public:
    SequentialStrategy( const WaveProcessingParams & );

    virtual double * operator()() override;


    virtual std::string getStrategyName() override;

private:
    WavePocessing _proc;
};

#endif