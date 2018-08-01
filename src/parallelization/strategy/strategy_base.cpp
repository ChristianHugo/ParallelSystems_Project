#include "parallelization/strategy/strategy_base.h"

#include "processing/wave_processing_params.h"

using namespace std;


/*!
    \brief Initalisiert das Strategie-Basis Objekt und speichert die Parameter
 */
StrategyBase::StrategyBase( const WaveProcessingParams &params ) : 
    _params( params )
{}


/*!
    \brief Liefert die Anzahl der Threads aus den Parametersn
 */
int StrategyBase::getNumThreads() const 
{ 
    return _params.numThreads; 
}


/*!
    \brief Liefert die Dauer der Simulation aus den Parametersn
 */
int StrategyBase::getTimeLenght() const 
{ 
    return _params.nt; 
}


/*!
    \brief Liefert die Länge der Saite aus den Parametersn
 */
int StrategyBase::getPositionLength() const 
{ 
    return _params.nx; 
}


/*!
    \brief setzt den Anfangswert für die Zeitmessung
 */
void StrategyBase::initStartTime()
{
    _startTime = chrono::high_resolution_clock::now();
}


/*!
    \brief führt die Zeitmessung aus
 */
void StrategyBase::calcDuration()
{
    auto endTime = chrono::high_resolution_clock::now();
    _duration = chrono::duration< double, milli > ( endTime - _startTime ).count();
}


/*!
    \brief liefert die berechnete Zeitdauer in Millisekunden zurück
 */
double StrategyBase::getDuration() const
{
    return _duration;
}
