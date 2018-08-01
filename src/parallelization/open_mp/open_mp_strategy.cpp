#include "parallelization/open_mp/open_mp_strategy.h"

#include "parallelization/strategy/strategy_type.h"
#include "processing/wave_processing.h"
#include "processing/wave_processing_params.h"

#include <algorithm>
#include <iostream>

#include <omp.h>


using namespace std;


/*!
    \brief Konstruktor mit WaveProcessingParams
    
    Initialisiert das Objekt, reicht params an die Basisklasse weiter
    und erzeugt ein WavePocessing Objekt für die Berechnung.
 */
OpenMPStrategy::OpenMPStrategy( const WaveProcessingParams &params ) :
    StrategyBase( params ),
    _proc( WavePocessing( params ) )
{}


/*!
    \brief Einstiegspunkt zu Parallelisierung

    überschriebener Operator von der Basisklasse.
    Startet die Strategie
 */
double * OpenMPStrategy::operator()() 
{ 
    double *waveData = new double[ _params.nt * _params.nx ];
    fill( waveData, waveData + _params.nt * _params.nx, 0.0 );
    
    _proc.doICAndBC( waveData );

    // Zeitmessung starten    
    initStartTime();
    #pragma omp parallel num_threads( getNumThreads() )
    for ( int i = 2; i < _params.nt; i++ )
    {
        #pragma omp for schedule( static )
        for ( int j = 1; j < _params.nx - 1; j++ )
            _proc.process( i, j, waveData );
    }
    // Dauer berechnen
    calcDuration();
    return waveData;
}


/*!
    \brief Liefert OpenMPStrategy als String
 */
string OpenMPStrategy::getStrategyName() 
{
    return "OpenMPStrategy";
}