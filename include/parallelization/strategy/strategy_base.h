#ifndef STRATEGY_BASE
#define STRATEGY_BASE

#include <chrono>
#include <memory>
#include <string>

using namespace std::chrono;

class WaveProcessingParams;


/*!
    \brief Basisklasse für alle Strategien

    Definiert die Basisklasse für Strategien aus dem Stratie Entwurfsmuster.
    Ableitende Klassen approximieren die Wellengleichung mit unterschiedlichern
    Ausführungsstrategien. Sie überschreiben den () operator als Einstiegspunkt.
 */
class StrategyBase {
public:
    StrategyBase( const WaveProcessingParams & );

    //! default Destruktor
    virtual ~StrategyBase() = default;

    /*! 
        \brief Einstiegspunkt zu einer Strategie

        Ableitende Strategien müssen den () Operator überschreiben,
        damit sie über die abstrakte Schnittstelle gestartet werden können.
    */
    virtual double * operator()() = 0;
    //! um den Namen der Strategie lesen zu können
    virtual std::string getStrategyName() = 0;

    int getNumThreads() const;
    int getTimeLenght() const;
    int getPositionLength() const;

    void initStartTime();
    void calcDuration();
    double getDuration() const;

    //! Abkürzung für den unique_ptr< StrategyBase >
    using ptr = std::unique_ptr< StrategyBase >;

protected:
    //! aus der Konfiguration erzeugtes Parameterobjekt
    const WaveProcessingParams &_params;

private:
    high_resolution_clock::time_point _startTime;
    double _duration;
};

#endif