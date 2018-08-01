#ifndef _CFG_PARSER
#define _CFG_PARSER

#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include <boost/optional.hpp>

enum class STRATEGY_TYPE;

/*!
    \brief Parser um Konfigurations-Parameter auszulesen

    Liest mithilfe von regulären Ausdrücken Parameter aus einem String.
    Die Parameter müssen die Form

      ParameterName=ParameterWert
    
    haben. Bspw.
      execution_strategy=0
    für eine sequentielle Ausführung. Ein gefundener Wert wird als boost::optional< T >
    zurückgegeben oder boost::None.
    
    Leerzeichen sind nicht unterstützt, bei doppelten Parameter wird der erste genommen.
 */
class CfgParser
{
public:
    CfgParser( const std::string & );

    boost::optional< STRATEGY_TYPE > getStrategyType() const;
    boost::optional< int > getNumThreads() const;

    boost::optional< double > getWaveVelo() const;
    boost::optional< double > getFreq() const;
    boost::optional< double > getLength() const;
    boost::optional< double > getTime() const;
    boost::optional< double > getNt() const;
    boost::optional< double > getNx() const;

private:
    std::string getCfgVal( const std::regex &regExp ) const;

    std::string _lines;

    std::regex execStratRegex = 
        std::regex( "execution_strategy=(.*)", std::regex_constants::icase );
    std::regex numThreadsRegex = 
        std::regex( "num_threads=(.*)", std::regex_constants::icase );

    std::regex waveVeloRegex = 
        std::regex( "wave_velocity=(.*)", std::regex_constants::icase );
    std::regex freqRegex = 
        std::regex( "frequency=(.*)", std::regex_constants::icase );
    std::regex lengthRegex = 
        std::regex( "length=(.*)", std::regex_constants::icase );
    std::regex timeRegex = 
        std::regex( "time_duration=(.*)", std::regex_constants::icase );
    std::regex ntRegex = 
        std::regex( "nt=(.*)", std::regex_constants::icase );
    std::regex nxRegex = 
        std::regex( "nx=(.*)", std::regex_constants::icase );
};

#endif