#include "processing/wave_processing.h"

#include "processing/wave_processing_params.h"

#include <cmath>
#include <math.h>
#include <iostream>

using namespace std;


/*!
    \brief Instanziiert mit einem WaveProcessingParams Objekt.

    Speichert die Parameter für die Approximerung, 
    damit sie bspw. in pcrocess() benutzt werden können.

    \param processParams Parameter für die Approximierung
 */
WavePocessing::WavePocessing( const WaveProcessingParams &processParams ) : 
    _processParams( processParams )
{}


/*!
    \brief Approximiert anhand der Koordinaten

    Berechnet das approximierte Ergebnis der Wellengleichung
    an einer (t,x) Koordinate und setzt den Wert in waveData. 
    Die Methode holt sich die Abhängigkeiten aus waveData, 
    delegiert zu der anderen process() Methode und schreibt
    das Ergebnis nach waveDate.

    \param t Zeit
    \param x Position
    \param waveData Matrix M
 */
void WavePocessing::process( int t, int x, double *waveData ) 
{
    double newVal = 2 * getAt( t - 1, x, waveData ) - getAt( t - 2, x, waveData ) + 
        _processParams.r * ( getAt( t - 1, x + 1, waveData ) - 
        2 * getAt( t - 1, x, waveData ) + getAt( t - 1, x - 1, waveData ) );
    setAt( t, x, waveData, newVal ); 
}

/*!
    \brief Approximiert anhand der Abhängigkeiten

    Berechnet das approximierte Ergebnis der Wellengleichung
    und nimmt die Abhängigkeiten als separate double Variablen entgegen.

    \param M_t_xMinus1 vorherige Zeile, linke Spalte
    \param M_t_x vorherige Zeile, mittige Spalte
    \param M_t_xPlus1 vorherige Zeile, rechte Spalte
    \param M_tMinus1_x vor-vorherige Zeile, mittige Spalte
 */
double WavePocessing::process( double M_t_xMinus1, double M_t_x, 
                               double M_t_xPlus1, double M_tMinus1_x )
{
    return 2 * M_t_x - M_tMinus1_x + _processParams.r * 
        ( M_t_xPlus1 - 2 * M_t_x + M_t_xMinus1 );
}

/*!
    \brief setzt die initial und boundary conditions in waveData

    benutzt initialConditions für die IC sowie boundaryConditionsLeft
    und boundaryConditionsRight für die BC.

    \param waveData Matrix M

 */
void WavePocessing::doICAndBC( double *waveData )
{
    for ( int i = 1; i < _processParams.nx - 2; i++ )
        initialConditions( 1, i, waveData );

    for ( int i = 1; i < _processParams.nt - 1; i++ )
    {
        boundaryConditionsLeft( i, waveData );
        boundaryConditionsRight( i, waveData );
    }
}


/*!
    \brief Setzt die initial conditions in waveData

    \param t Zeit
    \param x Position
    \param waveData Matrix M
 */
void WavePocessing::initialConditions( int t, int x, double *waveData )
{ 
    double newVal = getAt( t - 1,  x, waveData ) + 0.5 * _processParams.r *
            ( getAt( t - 1, x + 1, waveData ) - 2.0 * getAt( t - 1, x, waveData ) + 
            getAt( t - 1, x -1, waveData ) );
    setAt( t, x, waveData, newVal );
}


/*!
    \brief Setzt eine linke BC in waveData

    \param t Zeitkoordinate, an der die BC gesetzt wird
    \param waveData Matrix M
 */
void WavePocessing::boundaryConditionsLeft( int t, double *waveData )
{
    double timeVal = t * _processParams.dt;
    double displace = 3 * sin( 2 * M_PI * _processParams.frequency * timeVal ) * 
            exp( -1.0 * _processParams.time_duration );
    setAt( t, 0, waveData, displace );
}


/*!
    \brief Setzt die linken BC in waveData

    \param t Zeitkoordinate, an der die BC gesetzt wird
    \param waveData Matrix M
 */
void WavePocessing::boundaryConditionsRight( int t, double *waveData ) 
{
    setAt( t, _processParams.nx, waveData, 0 );
}


/*!
    \brief Hilfsmethode

    Hilfsmethode um aus dem flachen double * Array mit
    zweidimensionalen Koordinaten zugreifen zu können

    \param i Zeile
    \param j Spalte
    \param data flaches array, aus dem eine Zelle zu lesen ist
 */
double WavePocessing::getAt( int i, int j, double *data )
{
    int rowOffset = i * _processParams.nx;
    return data[ rowOffset + j ];
}


/*!
    \brief Hilfsmethode

    Hilfsmethode um in einem flachen array eine Zelle mit zweidimensionalen
    Koordinaten zu addressieren und zu setzen.

    \param i Zeile
    \param j Spalte
    \param data flaches array, in dem ein Wert zu setzen ist
    \param value zu setzender Wert
 */
void WavePocessing::setAt( int i, int j, double *data, double value )
{
    int rowOffset = i * _processParams.nx;
    data[ rowOffset + j ] = value;
}