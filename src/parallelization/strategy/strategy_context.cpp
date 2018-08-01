#include "parallelization/strategy/strategy_context.h"

#include "parallelization/open_mp/open_mp_strategy.h"
#include "parallelization/strategy/strategy_base.h"
#include "parallelization/std/std_threads_strategy.h"
#include "parallelization/strategy/strategy_type.h"
#include "processing/wave_processing.h"
#include "processing/wave_processing_params.h"
#include "parallelization/strategy/strategy_factory.h"

#include <algorithm>
#include <ctime>
#include <iostream>
#include <sstream> 
#include <utility>

using namespace std;


/*!
    \brief Schnittstelle um eine Strategie zu starten

    Nimmt den Strategie Typen sowie Parameter entgegen und führt die Strategie aus.
    Die Ergebnisse werden als flaches double * array zurückgeliefert.
    
    \param type auszuführende Strategie
    \param params Parameter für die AUsführung

    \note Auch wenn der STRATEGY_TYPE ebenfalls in params steht wird er gesondert
    übergeben um die Schnittstelle sprechender zu machen.
 */
double * StrategyContext::executeStrategy( STRATEGY_TYPE type,
                                           const WaveProcessingParams &params )

{
    StrategyBase::ptr strategy = strategy_n::factory( type, params );
    return executeStrategy( move( strategy ) );
}


/*!
    \brief privater Helfer
    Führt die Strategie aus und Speichert die Laufzeit
 */
double * StrategyContext::executeStrategy( StrategyBase::ptr strategy )
{
    if ( strategy )
    {
        double *result = ( *strategy )();
        insertRunStats( move( strategy ) );
        return result;
    }
    return  nullptr;
}


/*!
    \brief Speichert Laufzeit-Infos als String

    Es wird der Name der Strategie, die größe der Matrix M 
    sowie die Laufzeit selbst gespeichert.
 */
void StrategyContext::insertRunStats( StrategyBase::ptr strat )
{
    ostringstream oss;
    oss << strat->getStrategyName() << " " 
        << strat->getNumThreads() << " " 
        << strat->getTimeLenght() << " " 
        << strat->getPositionLength() << " " 
        << strat->getDuration(); 
    _runStats.push_back( oss.str() );
}


/*!
    \brief Schreibt gespeicherte Laufzeit-Infos auf die Standardausgabe
 */
void StrategyContext::dumpRunStats()
{
    for ( const auto &line : _runStats )
        cout << line << endl;
}
