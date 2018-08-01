#include "cfg/cfg_util.h"
#include "gui/open_gl_gui.h"
#include "parallelization/strategy/strategy_context.h"
#include "processing/wave_processing_params.h"
#include "parallelization/strategy/strategy_type.h"


/*
    Mainfunktion
        liest die Konfiguration
        ruft die konfigurierte Strategie auf
        schreibt die Laufzeit auf die Standardausgabe
        initalisiert die GUI
 */
int main( int argc, char **argv) 
{
    WaveProcessingParams params = cfg_file_util_n::buildCfg( argc, argv );

    StrategyContext stratCtx;
    double *data = stratCtx.executeStrategy( params.type, params );
    
    stratCtx.dumpRunStats();

    open_gl_gui_n::initGUI( argc, argv, data, params.nt, params.nx );
    
    return 0;
}