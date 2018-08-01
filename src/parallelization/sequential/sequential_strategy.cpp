#include "parallelization/sequential/sequential_strategy.h"

#include "processing/wave_processing_params.h"
#include "processing/wave_processing.h"

#include <algorithm>
#include <iostream>

using namespace std;

/*!
    \brief Konstruktor mit WaveProcessingParams
    
    Initialisiert das Objekt, reicht params an die Basisklasse weiter
    und erzeugt ein WavePocessing Objekt für die Berechnung.
 */
SequentialStrategy::SequentialStrategy( const WaveProcessingParams &params ) :
    StrategyBase( params ),
    _proc( WavePocessing( _params ) )
{}


/*!
    \brief Einstiegspunkt zum sequentiellen approximieren
 */
double * SequentialStrategy::operator()()
{
    // Speicher vorbereiten
    double *waveData = new double[ _params.nt * _params.nx ];
    fill( waveData, waveData + _params.nt * _params.nx, 0.0 );
    
    _proc.doICAndBC( waveData );

    // Zeitmessung starten    
    initStartTime();
    // pro Zeitschritt über alle Positionen 
    for ( int i = 2; i < _params.nt; i++ ) 
        for ( int j = 1; j < _params.nx - 1; j++ ) 
            _proc.process( i, j, waveData );
    // Dauer berechnen
    calcDuration();
    return waveData;
}


/*!
    \brief Liefert SequentialStrategy als String
 */
string SequentialStrategy::getStrategyName()
{
    return "SequentialStrategy";
}
