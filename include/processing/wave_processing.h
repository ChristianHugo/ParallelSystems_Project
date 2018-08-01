#ifndef WAVE_PROCESSING_FUNC
#define WAVE_PROCESSING_FUNC

struct WaveProcessingParams;

/*!
    \brief Klasse zum approximieren der Wellengleichung.
    
    Die Klasse implementiert zentral die numerische Approximierung der Wellengleichung.
    Sie wird von den Ausführungsstrategien benutzt. 
    Nachdem sie mit einem WaveProcessingParams Objekt instanziiert wurde, 
    kann eine der process() Methoden aufgerufen werden.
 */
class WavePocessing 
{
public:
    WavePocessing( const WaveProcessingParams & );
    
    //! default Destruktor
    ~WavePocessing() = default;
    double process( double, double, double, double );


    void process( int, int, double * );

    void doICAndBC( double * );
    void initialConditions( int, int, double * );
    void boundaryConditionsLeft( int, double * );
    void boundaryConditionsRight( int, double * );

    //! Parameter für die Approximierung
    const WaveProcessingParams &_processParams;

private:
    double getAt( int, int, double * );
    void setAt( int, int, double *, double );

};

#endif