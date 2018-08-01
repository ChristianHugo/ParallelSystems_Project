#ifndef _OPEN_MP_STRATEGY
#define _OPEN_MP_STRATEGY

#include "parallelization/strategy/strategy_base.h"
#include "processing/wave_processing.h"

#include <string>

class WaveProcessingParams;


/*!
    \brief mit OpenMP parallelisierte Strategie

    Leitet von StrategyBase ab und parallelisiert das Approximieren 
    mithilfe der OpenMP Bibliothek.
 */
class OpenMPStrategy : public StrategyBase 
{
public:
    OpenMPStrategy( const WaveProcessingParams & );

    virtual double * operator()() override;
    virtual std::string getStrategyName() override;
private:
    WavePocessing _proc;
};

#endif