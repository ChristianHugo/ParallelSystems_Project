#define BOOST_TEST_MODULE StrategiesTest


#include <boost/test/framework.hpp>
#include <boost/test/included/unit_test.hpp>

#include <iostream>
#include <string>

#include "strategy_tester.h"
#include "cfg/cfg_util.h"
#include "processing/wave_processing_params.h"
#include "parallelization/strategy/strategy_type.h"


using namespace std;

namespace
{
    void testFuncFixedSize( STRATEGY_TYPE type, int numThreads ) 
    {
        StrategyTester tester( 4, 4, numThreads );
        double *matrix = tester.executeWaveProcessing( type );
        tester.checkMatrix( matrix );
        delete[] matrix;
    }

    void testFuncMultibleSizes( STRATEGY_TYPE type, int numThreads ) 
    {
        for ( int rows = 7; rows < 12; rows++ )
        {
            for ( int cols = 7; cols < 12; cols++)
            {        
                StrategyTester tester( rows, cols, numThreads );
                double *matrix = tester.executeWaveProcessing( type );
                tester.checkMatrix( matrix );
                delete[] matrix;
            }
        }
    }
}


// ----------------------------------------------------
BOOST_AUTO_TEST_SUITE( parallelization_suite )


// std threads
BOOST_AUTO_TEST_CASE( std_strategy ) 
{
    testFuncFixedSize( STRATEGY_TYPE::STD_THREAD, 3 );
}
BOOST_AUTO_TEST_CASE( std_strategy_single ) 
{
    testFuncFixedSize( STRATEGY_TYPE::STD_THREAD, 1 );
}

// openMP
BOOST_AUTO_TEST_CASE( open_mp_strategy ) 
{
    testFuncFixedSize( STRATEGY_TYPE::OPEN_MP, 3 );
}
BOOST_AUTO_TEST_CASE( open_mp_strategy_single_thread ) 
{
    testFuncFixedSize( STRATEGY_TYPE::OPEN_MP, 1 );
}

// std threads
BOOST_AUTO_TEST_CASE( multible_data_size_std ) 
{
    testFuncMultibleSizes( STRATEGY_TYPE::STD_THREAD, 3 );
}
BOOST_AUTO_TEST_CASE( multible_data_size_std_single_thread ) 
{
    testFuncMultibleSizes( STRATEGY_TYPE::STD_THREAD, 1 );
}

// openMP
BOOST_AUTO_TEST_CASE( multible_data_size_open_mp ) 
{
    testFuncMultibleSizes( STRATEGY_TYPE::OPEN_MP, 3 );
}
BOOST_AUTO_TEST_CASE( multible_data_size_open_mp_single_thread ) 
{
    testFuncMultibleSizes( STRATEGY_TYPE::OPEN_MP, 1 );
}

BOOST_AUTO_TEST_SUITE_END()


// ----------------------------------------------------
BOOST_AUTO_TEST_SUITE( cfg_parser_suite )


BOOST_AUTO_TEST_CASE( cfg_file_test_all_parameters_set ) 
{
    string lines =
        "wave_velocity=1\n"
        "frequency=2\n"
        "length=3\n"
        "time_duration=4\n"
        "nt=5\n"
        "nx=6\n";
    WaveProcessingParams result = cfg_file_util_n::buildCfgFromString( lines );

    BOOST_CHECK_EQUAL( result.wave_velocity, 1 );
    BOOST_CHECK_EQUAL( result.frequency, 2 );
    BOOST_CHECK_EQUAL( result.length, 3 );
    BOOST_CHECK_EQUAL( result.time_duration, 4 );
    BOOST_CHECK_EQUAL( result.nt, 5 );
    BOOST_CHECK_EQUAL( result.nx, 6 );
}


BOOST_AUTO_TEST_CASE( cfg_file_test_some_parameters_set ) 
{

    string lines =
        "wave_velocity=1\n"
        "length=3\n"
        "nt=5\n";
    WaveProcessingParams result = cfg_file_util_n::buildCfgFromString( lines );

    WaveProcessingParams defaultParams;
    BOOST_CHECK_EQUAL( result.wave_velocity, 1 );
    BOOST_CHECK_EQUAL( result.frequency, defaultParams.frequency );
    BOOST_CHECK_EQUAL( result.length, 3 );
    BOOST_CHECK_EQUAL( result.time_duration, defaultParams.time_duration );
    BOOST_CHECK_EQUAL( result.nt, 5 );
    BOOST_CHECK_EQUAL( result.nx, defaultParams.nx );
}


BOOST_AUTO_TEST_CASE( cfg_file_test_no_parameters_set ) 
{

    string lines;
    WaveProcessingParams result = cfg_file_util_n::buildCfgFromString( lines );

    WaveProcessingParams defaultParams;
    BOOST_CHECK_EQUAL( result.wave_velocity, defaultParams.wave_velocity );
    BOOST_CHECK_EQUAL( result.frequency, defaultParams.frequency );
    BOOST_CHECK_EQUAL( result.length, defaultParams.length );
    BOOST_CHECK_EQUAL( result.time_duration, defaultParams.time_duration );
    BOOST_CHECK_EQUAL( result.nt, defaultParams.nt );
    BOOST_CHECK_EQUAL( result.nx, defaultParams.nx );
}

BOOST_AUTO_TEST_SUITE_END()


/*
BOOST_AUTO_TEST_CASE( benchmark_test_wave_proc ) 
{
    StrategyTester tester( 10, 10, 1, false );

    auto innerTestFunc = [&]( int rows, int cols, int numThreads ) 
    {
        cout << "rows " << rows << ", cols " << cols << endl;
        tester.resetMatrix( rows, cols );
        tester.resetNumThreads( numThreads );
        tester.executeWaveProcessing( STRATEGY_TYPE::STD_THREAD );
        tester.executeWaveProcessing( STRATEGY_TYPE::OPEN_MP );
        tester.executeWaveProcessing( STRATEGY_TYPE::SEQUENTIAL );
        tester.dumpRunStats();
        cout << endl << "###########################################" << endl;
    };

    for ( int i = 100; i < 15100; i += 200 )
        innerTestFunc( i, i, 4);
}


BOOST_AUTO_TEST_CASE( benchmark_test_dummy_proc ) 
{
    StrategyTester tester( 10, 10, 1, false );

    auto innerTestFunc = [&]( int rows, int cols, int numThreads ) 
    {
        cout << "rows " << rows << ", cols " << cols << endl;
        tester.resetMatrix( rows, cols );
        tester.resetNumThreads( numThreads );
        tester.executeWaveProcessing( STRATEGY_TYPE::STD_THREAD );
        tester.executeWaveProcessing( STRATEGY_TYPE::OPEN_MP );
        tester.executeWaveProcessing( STRATEGY_TYPE::SEQUENTIAL );
        tester.dumpRunStats();
        cout << endl << "###########################################" << endl;
    };

    for ( int i = 100; i < 900; i += 200 )
        innerTestFunc( i, i, 4);
}
*/