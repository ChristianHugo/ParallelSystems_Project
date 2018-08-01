#include "parallelization/strategy/strategy_factory.h"

#include "parallelization/std/std_threads_strategy.h"
#include "parallelization/open_mp/open_mp_strategy.h"
#include "parallelization/sequential/sequential_strategy.h"

#include "parallelization/strategy/strategy_type.h"

#include "processing/wave_processing_params.h"

#include <utility>

using namespace std;


/*!
    \brief factory Funktion um eine Strategie zu erzeugen

    Erzeugt eine konkrete Strategie und liefert sie als Typ von StrategyBase zurück. 
    Um die Strategie Polymorph zurückliefern zu können wurden unique_ptr benutzt.
    unique_ptr haben gegenüber von raw pointern den Vorteil, 
    dass sie nicht von Hand zu löschen sind.

    \param type enum Wert der die Strategie indentifiziert
    \param param Parameter aus der Konfiguration oder die Defaultwerte
 */
StrategyBase::ptr strategy_n::factory( STRATEGY_TYPE type, const WaveProcessingParams &params )
{
    if ( type == STRATEGY_TYPE::STD_THREAD )
        return make_unique< StdThreadsStrategy > ( params );
    else if ( type == STRATEGY_TYPE::OPEN_MP )
        return make_unique< OpenMPStrategy > ( params );
    else if ( type == STRATEGY_TYPE::SEQUENTIAL )
        return make_unique< SequentialStrategy >( params );
    return StrategyBase::ptr( nullptr );
}
