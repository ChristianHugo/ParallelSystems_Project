#ifndef _STRATEGY_FACTORY
#define _STRATEGY_FACTORY

#include "parallelization/strategy/strategy_base.h"

enum class STRATEGY_TYPE;
class WaveProcessingParams;

namespace strategy_n
{
    StrategyBase::ptr factory( STRATEGY_TYPE, const WaveProcessingParams & );
}
#endif