#include "cfg/cfg_util.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <regex>

#include "cfg/cfg_parser.h"

using namespace std;

namespace
{
    /*
        Helfer um die Werte vom Parser im WaveProcessingParams Objekt "result" zu setzen
     */
    void setValuesFromParser( const CfgParser &parser, WaveProcessingParams &result )
    {
        if ( auto waveVelo = parser.getWaveVelo() )
            result.wave_velocity = *waveVelo;
        if ( auto freq = parser.getFreq() )
            result.frequency = *freq;
        if ( auto length = parser.getLength() )
            result.length = *length;
        if ( auto time = parser.getTime() )
            result.time_duration = *time;
        if ( auto ntOpt = parser.getNt() )
            result.nt = *ntOpt;
        if ( auto nx = parser.getNx() )
            result.nx = *nx;

        if ( auto stratType = parser.getStrategyType() )
            result.type = *stratType;
        if ( auto numThreads = parser.getNumThreads() )
            result.numThreads = *numThreads;
    }
}


/*!
    \brief erzeugt ein Parameterobjekt

    Wenn in den argv Parametern ein cfg_file Parameter steht, wird aus der Datei gelesen.
    Ansonsten werden die Parameter in den Kommandozeilenparametern geparst und
    nicht gesetzte Werte auf dem default gelassen.

    \param argc Länge der Parameter
    \param argv Parameter array von char * Strings
 */
WaveProcessingParams cfg_file_util_n::buildCfg( int argc, char **argv )
{
    std::regex cfgFileRegex( "cfg_file=(.*)", std::regex_constants::icase );
    for ( int i = 1; i < argc; i++ )
    {
        smatch match;
        string cmdlPara( argv[ i ] );
        if ( regex_search( cmdlPara, match, cfgFileRegex ) )
        {
            // aus Konfigurationsdatei
            string cfgFile = match.str( 1 );
            return cfg_file_util_n::buildCfgFromFile( cfgFile );
        }
    }

    // aus Kommandozeilenparametern
    return cfg_file_util_n::buildCfgFromCmdLines( argc, argv );
}


/*!
    \brief erzeugt Parameter-Objekt aus Konfigurationsdatei

    \param location Pfad zur Konfigurationsdatei
 */
WaveProcessingParams cfg_file_util_n::buildCfgFromFile( const std::string &location )
{
    std::ifstream ifs;
    ifs.open ( location, std::ifstream::in);
    string allLines;
    std::string nextLine;
    while ( std::getline( ifs, nextLine ) )
    {
        if ( !nextLine.empty() )
            allLines.append( nextLine );
    }
    return buildCfgFromString( allLines );
}


/*!
    \brief erzeugt Parameter-Objekt aus einem String

    \param lines String mit dem Inhalt Konfigurationsdatei oder den
    kombinierten Kommandozzeilenparametern
 */
WaveProcessingParams cfg_file_util_n::buildCfgFromString( const std::string &lines )
{    
    CfgParser parser( lines );

    WaveProcessingParams result;
    setValuesFromParser( parser, result );
    result.init();

    return result;
}


/*!
    \brief erzeugt Parameter-Objekt aus Kommandozeilenparametern
    
    \param argc Länge der Kommandozeilenparameter
    \param argv array der Kommandozeilenparameter als char *
 */
WaveProcessingParams cfg_file_util_n::buildCfgFromCmdLines( int argc, char **argv )
{
    string cmdParams;
    for ( int i = 1; i < argc; i++ )
        cmdParams.append( argv[ i ] ).append( "\n" );
    return cfg_file_util_n::buildCfgFromString( cmdParams );
}
