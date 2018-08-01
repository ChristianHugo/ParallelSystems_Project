#ifndef _CFG_UTIL
#define _CFG_UTIL

#include <string>
#include <vector>

#include "processing/wave_processing_params.h"


namespace cfg_file_util_n
{
    WaveProcessingParams buildCfg( int, char ** );

    WaveProcessingParams buildCfgFromFile( const std::string & );
    WaveProcessingParams buildCfgFromString( const std::string & );
    WaveProcessingParams buildCfgFromCmdLines( int, char ** );
}

#endif