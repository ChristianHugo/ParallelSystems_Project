#ifndef _WAVE_PROCESSING_PARAMS
#define _WAVE_PROCESSING_PARAMS

#include "parallelization/strategy/strategy_type.h"

/*!
    \brief umwickelt alle Programmparameter

    Beinhaltet alle Parameter, die entweder aus einer Konfigurationsdatei
    oder den Kommandozeilenparametern kommen. Die Standardwerte sind hier
    direkt eingetragen und können durch die Konfiguration überschrieben werden.

    \note Auch wenn der STRATEGY_TYPE gesondert übergeben wird, steht er noch einmal hier,
    denn er stammt auch aus der Konfiguration.
 */
struct WaveProcessingParams
{
    //! Ausbreitungsgeschwindigkeit
    double wave_velocity = 6;
    //! Wellen-Frequenz
    double frequency = 3;
    //! Länge der Saite
    double length = 10.0;
    //! Dauer der Simulation
    double time_duration = 10.0;
    //! Anzahl diskreter Zeitschritte
    int nt = 1000;
    //! Anzahl diskreter Positionsschritte
    int nx = 400; 
    //! Delta x ( length / nx )
    double dx;
    //! Delta t ( time_duration / nt )
    double dt;
    //!
    double r;

    //! Ausführungsstrategie aus der Konfiguration
    STRATEGY_TYPE type = STRATEGY_TYPE::SEQUENTIAL;
    //! Anzahl Threads aus der Konfiguration
    int numThreads = 1;

    //! default Konstruktor
    WaveProcessingParams() = default;
    //! Aktualisiert dx, dt und r
    void init();
    //! Schreibt die Parameter auf die Ausgabe
    void dump();

};

#endif