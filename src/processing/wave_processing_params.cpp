#include "processing/wave_processing_params.h"

#include <cmath>
#include <iostream>

using namespace std;

void WaveProcessingParams::init()
{
    dx = length / ( nx - 1 );
    dt = time_duration / ( nt - 1 );
    r = wave_velocity * pow( ( dt / dx ), 2 );
}


void WaveProcessingParams::dump()
{
    cout << "wave_velocity " << wave_velocity << endl
         << "frequency " << frequency << endl
         << "length " << length << endl
         << "time_duration " << time_duration << endl
         << "nt " << nt << endl
         << "nx " << nx << endl
         << "dx " << dx << endl
         << "dt " << dt << endl
         << "r " << r << endl;
}