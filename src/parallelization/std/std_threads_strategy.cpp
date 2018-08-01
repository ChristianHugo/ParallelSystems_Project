#include "parallelization/std/std_threads_strategy.h"

#include "processing/wave_processing.h"
#include "processing/wave_processing_params.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>

using namespace std;


/*!
    \brief Konstruktor mit WaveProcessingParams
    
    Initialisiert das Objekt, reicht params an die Basisklasse weiter
    und erzeugt ein WavePocessing Objekt für die Berechnung.
 */
StdThreadsStrategy::StdThreadsStrategy( const WaveProcessingParams &params ) : 
    StrategyBase( params ),
    _barrier( getNumThreads() ),
    _proc( WavePocessing( _params ) )
{}


/*!
    \brief Einstiegspunkt zu Parallelisierung

    überschriebener Operator von der Basisklasse.
    Startet die Strategie
 */
double * StdThreadsStrategy::operator()()
{
    // Speicher vorbereiten
    double *waveData = new double[ _params.nt * _params.nx ];
    fill( waveData, waveData + _params.nt * _params.nx, 0.0 );

    _proc.doICAndBC( waveData );

    // Zeitmessung starten    
    initStartTime();

    int intervall = _params.nx / _params.numThreads;
    vector < thread > threadList;
    int numThreads = getNumThreads();
    for ( int i = 0; i < numThreads; i++ )
    {
        // partitionsgrenzen berechnen
        int xStart = i > 0 ? i * intervall + 1 : 1;
        int xEnd = i < ( numThreads - 1 ) ? 
            xStart + intervall : _params.nx - 1;
        // thread in processPartial() starten
        threadList.push_back( thread( 
            &StdThreadsStrategy::processPartial, this, 
            xStart, xEnd, waveData ) );
    }

    for ( auto &thread : threadList )
        thread.join();

    // Dauer berechnen
    calcDuration();
    return waveData;
}


/*!
    \brief privater Helfer

    Gestartete Threads haben ihren Einstiegspunkt in processPartial()

    \param xStart Partitionsstart
    \param xEnd Partitionsende
    \param waveData flaches array der Matrix M
 */ 
void StdThreadsStrategy::processPartial( int xStart, int xEnd, double *waveData )
{    
    for ( int i = 2; i < _params.nt; i++ ) 
    {
        for ( int j = xStart; j < xEnd; j++ )
            _proc.process( i, j, waveData );
        _barrier.wait();
    }
}


/*!
    \brief Liefer StdThreadsStrategy as String
 */
string StdThreadsStrategy::getStrategyName()
{
    return "StdThreadsStrategy";
}