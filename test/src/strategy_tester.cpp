#include "strategy_tester.h"

#include "parallelization/open_mp/open_mp_strategy.h"
#include "parallelization/strategy/strategy_factory.h"
#include "processing/wave_processing_params.h"

#include <algorithm>
#include <iostream>
#include <math.h>
#include <utility>

#include <boost/test/test_tools.hpp>


using namespace std;


/*!

 */
StrategyTester::StrategyTester( int rows, int cols, 
                                int numThreads ) :
    _ctx(),
    _rows( rows ),
    _columns( cols ),
    _numThreads( numThreads ){
    _expectedData = executeWaveProcessing( STRATEGY_TYPE::SEQUENTIAL );
}


StrategyTester::~StrategyTester()
{
    delete[] _expectedData;
}

/*!

 */
double * StrategyTester::executeWaveProcessing( STRATEGY_TYPE type )
{
    WaveProcessingParams params;
    params.nt = _rows;
    params.nx = _columns;
    params.numThreads = _numThreads;
    params.init();
    return _ctx.executeStrategy( type, params );
}


/*!

 */
void StrategyTester::dumpRunStats()
{
    _ctx.dumpRunStats();
}


/*!

 */
void StrategyTester::checkMatrix( double *matrix )
{
    BOOST_CHECK( matrix );
    for ( int r = 0; r < _rows; r++ )
    {
        for ( int c = 0; c < _columns; c++ )
        {
            bool equal = getAt( r, c, _expectedData ) == getAt( r, c, matrix );
            BOOST_CHECK( equal );
            if ( !equal )
                cout << "expected: " << getAt( r, c, _expectedData ) << 
                        ",  result : " << getAt( r, c, matrix ) << endl;
        }
    }

    if ( _columns < 13 && _rows < 13 )
    {
        cout << endl << "       expected Matrix" << endl;
        for ( int i = 0; i < _rows; i++ )
        {
            int rowOffset = i * _columns;
            cout << "  ";
            for ( int j = 0; j < _columns; j++ )
                cout << _expectedData[ rowOffset + j ] << ", ";
            cout << endl;
        }
        

        cout << endl << "       result Matrix " << endl;
        for ( int i = 0; i < _rows; i++ )
        {
            int rowOffset = i * _columns;
            cout << "  ";
            for ( int j = 0; j < _columns; j++ )
                cout << matrix[ rowOffset + j ] << ", ";
            cout << endl;
        }
    }
}


/*!

 */
void StrategyTester::logExecuteStrategy( StrategyBase::ptr &strat )
{
    cout << "  Strategy: \"" << strat->getStrategyName() << "\", numThreads: " << _numThreads << 
        ", rows: " << _rows << ", cols: " << _columns << endl;
}


/*!

 */
void StrategyTester::resetMatrix( int rows, int cols )
{
    _rows = rows;
    _columns = cols;
    _expectedData = executeWaveProcessing( STRATEGY_TYPE::SEQUENTIAL );
}


/*!

 */
void StrategyTester::resetNumThreads( int numThreads )
{
    _numThreads = numThreads;
}


double StrategyTester::getAt( int i, int j, double *matrix )
{
    int rowOffset = i * _columns;
    return matrix[ rowOffset + j ];
}
