#include "cfg/cfg_parser.h"

#include "parallelization/strategy/strategy_type.h"

#include <iostream>
#include <stdexcept>

#include <boost/optional.hpp>


using namespace std;
using boost::optional;


/*!

 */
CfgParser::CfgParser( const std::string &lines ) : 
    _lines( lines )
{} 


/*!

 */
string CfgParser::getCfgVal( const std::regex &regExp ) const
{
    smatch match;
    if ( regex_search( _lines, match, regExp ) ) 
        return match.str( 1 );
    return string();
}


/*!

 */
boost::optional< STRATEGY_TYPE > CfgParser::getStrategyType() const
{
    try
    {
        int stratID = stoi( getCfgVal( execStratRegex )  );
        if ( stratID >= (int) STRATEGY_TYPE::START && stratID < (int) STRATEGY_TYPE::END )
            return ( STRATEGY_TYPE ) stratID;
    }
    catch ( const invalid_argument &e ) 
    { 
        //cout << "No valid stratey given. Using the default value." << endl;
    }
    return optional< STRATEGY_TYPE >(); 
}


/*!

 */
boost::optional< int > CfgParser::getNumThreads() const
{
    try { return stoi( getCfgVal( numThreadsRegex ) ); }
    catch ( const invalid_argument & ) 
    { 
        //cout << "No valid num threads given. Using the default value." << endl;
        return optional< int >(); 
    }
}


/*!

 */
optional< double > CfgParser::getWaveVelo() const
{ 
    try { return stod( getCfgVal( waveVeloRegex ) ); }
    catch ( const invalid_argument & ) 
    { 
        //cout << "No valid wave velocity given. Using the default value." << endl;
        return optional< double >(); 
    }
}


/*!

 */
optional< double > CfgParser::getFreq() const
{ 
    try { return stod( getCfgVal( freqRegex ) ); }
    catch ( const invalid_argument & ) 
    { 
        //cout << "No valid frequency given. Using the default value." << endl;
        return optional< double >(); 
    }
}


/*!

 */
optional< double > CfgParser::getLength() const
{ 
    try { return stod( getCfgVal( lengthRegex ) ); }
    catch ( const invalid_argument & ) 
    { 
        //cout << "No valid length given. Using the default value." << endl;
        return optional< double >(); 
    }
}


/*!

 */
optional< double > CfgParser::getTime() const
{ 
    try { return stod( getCfgVal( timeRegex ) ); }
    catch ( const invalid_argument & ) 
    {         
        //cout << "No valid time given. Using the default value." << endl;
        return optional< double >(); 
    }
}


/*!

 */
optional< double > CfgParser::getNt() const
{ 
    try { return stod( getCfgVal( ntRegex ) ); }
    catch ( const invalid_argument & ) 
    { 
        //cout << "No valid nt given. Using the default value." << endl;
        return optional< double >(); 
    }
}


/*!

 */
optional< double > CfgParser::getNx() const
{ 
    try { return stod( getCfgVal( nxRegex ) ); }
    catch ( const invalid_argument & ) 
    { 
        //cout << "No valid nx given. Using the default value." << endl;
        return optional< double >(); 
    }
}
