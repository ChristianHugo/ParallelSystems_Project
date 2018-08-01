#ifndef _STRATEGY_CONTEXT
#define _STRATEGY_CONTEXT

#include "parallelization/strategy/strategy_base.h"

#include <memory>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <boost/format.hpp>

class WaveProcessingParams;
enum class STRATEGY_TYPE;


/*!
    \brief Kontext um eine Strategie zu starten

    Implementiert den Kontext vom Strategiemuster. Der Kontext kenn alle Strategien
    und kann sie wahlweise ausführen.
 */
class StrategyContext 
{
public:
    //! default Konstruktor
    StrategyContext() = default;
    
    double * executeStrategy( STRATEGY_TYPE, const WaveProcessingParams & );

    void dumpRunStats();
private:
    double * executeStrategy( StrategyBase::ptr );
    void insertRunStats( StrategyBase::ptr );

    std::vector< std::string > _runStats;
};

#endif