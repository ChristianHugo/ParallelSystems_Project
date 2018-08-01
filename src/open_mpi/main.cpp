#include "cfg/cfg_util.h"
#include "processing/wave_processing_params.h"
#include "processing/wave_processing.h"
/**
@file main.cpp
*/
#include <algorithm>
#include <iostream>
#include <cstring>
#include <chrono>

#include <mpi.h>

using namespace std::chrono;
using namespace std;

int comm_size, comm_rank;


/*
    \brief namespace für die mit OpenMPI paralleliserte Implementierung
 */
namespace open_mpi_parallelization_n
{

    /*!
        \brief Initalisiert die MPI Umgebung
        \param argc Länge der Kommandozeilenparameter
        \param argv WErte der Kommandozeilenparameter
    */
    void init_mpi( int argc, char **argv ) 
    {
        int error_ret = MPI_Init( &argc, &argv );
        if( error_ret != MPI_SUCCESS ) 
        {
            cout << "Error while init" << endl;
            MPI_Abort( MPI_COMM_WORLD, error_ret );
        }
        error_ret = MPI_Comm_size( MPI_COMM_WORLD, &comm_size );
        if( error_ret != MPI_SUCCESS )
        {
            cout << "error while receiving the number of tasks.";
            MPI_Abort( MPI_COMM_WORLD, error_ret );
        }
        error_ret = MPI_Comm_rank( MPI_COMM_WORLD, &comm_rank );
        if( error_ret != MPI_SUCCESS ) 
        {
            cout << "error while receiving the rank of the current task." << endl;
            MPI_Abort( MPI_COMM_WORLD, error_ret );
        }  
    }


    /*!
        \brief zum prüfen ob der aktuelle Prozess der Master
    */
    int isRoot()
    {
        return comm_rank == 0;
    }


    /*!
        \brief erzeugt die Matrix M mit IC und BC
        \param nt Anzahl Zeitschritte
        \param nx ANzahl Positionen
        \param proc WavePocessing Objekt um die Formel anzuwenden
    */
    double *initWaveData( int nt, int nx, WavePocessing &proc )
    { 
        double *waveData = new double[ nt * nx ];
        fill( waveData, waveData + nt * nx, 0.0 );
        proc.doICAndBC( waveData );
        return waveData;
    }  


    /*!
        \brief teilt im Masterprozess mit MPI_Send Partitionen der letzten Zeile aus
        \param waveData Matrix M mit der letzten Zeile
        \param prevRow wird mit letzter Zeile befüllt, damit sich der Master wie ein Subprozess verhält
        \param prevRowOffset Index für die Zeile in das flache Array
        \param elementsPerProcess Partitionslänge 
    */
    void getPrevRowMaster( double *waveData, double *prevRow, 
                           int prevRowOffset, int elementsPerProcess )
    {
        int elemtsWithDeps = elementsPerProcess + 2;

        // über alle Prozesse bis auf den Master
        for ( int dest = 1; dest < comm_size; dest++ )
        {
            // Partition versenden
            int partitionStart = prevRowOffset + elementsPerProcess * dest;
            int errRet = MPI_Send( waveData + partitionStart, elemtsWithDeps, 
                                MPI_DOUBLE, dest, 0, MPI_COMM_WORLD );
            if ( errRet != MPI_SUCCESS )
            {
                cout << "Error while sending data partitions." << endl;
                MPI_Abort( MPI_COMM_WORLD, errRet );
            }
        }

        // der Master braucht kein Send
        std::memcpy( prevRow, waveData + prevRowOffset, sizeof ( double ) * elemtsWithDeps );
    }


    /*!
        \brief empfängt in den Subprozessen Partitionen der letzten Zeile
        \param prevRow Ausgabearray mit der Partition
        \param prevRowOffset Index für die Zeile in das flache Array
        \param elementsPerProcess Partitionslänge 
    */
    void getPrevRowWorker( double *prevRow, 
                           int prevRowOffset, int elementsPerProcess )
    {
        int elemtsWithDeps = elementsPerProcess + 2;

        // Partition empfangen und nach prevRow schreiben
        MPI_Status status;
        int errRet = MPI_Recv( prevRow, elemtsWithDeps, 
                            MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status );
        if ( errRet != MPI_SUCCESS )
        {
            cout << "Error while receiving data partitions." << endl;
            MPI_Abort( MPI_COMM_WORLD, errRet );
        }
    }


    /*!
        \brief Teilt die Partitionen der letzten Zeile auf die Subprozesse aus
        Nutzt intern getPrevRowMaster() und getPrevRowWorker()
        Damit die Partitionen überlapend verteilt werden können 
        werden MPI_Send() und MPI_Recv() benutzt.
    */
    void sendScatterPrevRow( double *waveData, double *prevRow, 
                             int prevRowOffset, int elementsPerProcess )
    {
        if ( isRoot() )
            getPrevRowMaster( waveData, prevRow, prevRowOffset, elementsPerProcess );
        else
            getPrevRowWorker( prevRow, prevRowOffset, elementsPerProcess );
    }


    /*!
        \brief ruft für die Partitionsgrenzen rocess() auf
        \param curRow Ausgabearray mit der aktuellen Zeile
        \param prevRow vorherige Zeile mit den Abnhängigkeiten
        \param prevPrevRow vor-vorherige Zeile mit den Abnhängigkeiten
        \param proc WavePocessing Objekt um die Formel anzuwenden
        \param elemtsWithDeps Partitionslänge plus den überlappenden Zellen
    */
    void calcCurPartition( double *curRow, double *prevRow, double *prevPrevRow, 
                           WavePocessing &proc, int elemtsWithDeps )
    {
        for ( int j = 1; j < elemtsWithDeps - 1; j++ )
        {
            curRow[ j ] = proc.process( 
                prevRow[ j - 1 ], prevRow[ j ], prevRow[ j + 1 ], prevPrevRow[ j ] );
        }
    }


    /*!
        \brief Gather Funktion um die Partitionen wieder zusammenzuführen
        \param waveData Matrix M in die die Daten zusammgeführt werden sollen
        \param curRow Partition der aktuellen Zeile
        \param elementsPerProcess Partitionslänge
        \param currentRowOffset wird benutzt um den Start in der Matrix M zu berechnen
    */
    void gather( double *waveData, double *curRow, 
                 int elementsPerProcess, int currentRowOffset )
    {

        int errRet = MPI_Gather( curRow + 1, elementsPerProcess, MPI_DOUBLE, 
                                 waveData + currentRowOffset + 1, 
                                 elementsPerProcess, MPI_DOUBLE, 
                                 0, MPI_COMM_WORLD );
        if ( errRet != MPI_SUCCESS )
        {
            cout << "Error while gathering data partitions." << endl;
            MPI_Abort( MPI_COMM_WORLD, errRet );
        }

    }
}

using namespace open_mpi_parallelization_n;

int main( int argc, char **argv )
{
    init_mpi( argc, argv );
    
    // alle Prozesse lesen die cfg
    // sönst müsste man das params Objekt verschicken
    WaveProcessingParams params = cfg_file_util_n::buildCfg( argc, argv );
    params.init();
    WavePocessing proc( params );

    // Matrix M, existiert nur im Master
    double *waveData = isRoot() ? initWaveData( params.nt, params.nx, proc ) : nullptr;

    int processLength = params.nx - 2;
    int elementsPerProcess = processLength / comm_size;
    int elemtsWithDeps = elementsPerProcess + 2;

    // Speicher vorbereiten
    double *curRow = new double[ elemtsWithDeps ];
    fill( curRow, curRow + elemtsWithDeps, 0.0 );
    double *prevRow = new double[ elemtsWithDeps ];
    fill( prevRow, prevRow + elemtsWithDeps, 0.0 );
    double *prevPrevRow = new double[ elemtsWithDeps ];
    fill( prevPrevRow, prevPrevRow + elemtsWithDeps, 0.0 );

    // für die Zeitmessung
    high_resolution_clock::time_point startTime;
    if ( isRoot() )
        startTime = chrono::high_resolution_clock::now();

    for ( int i = 2; i < params.nt; i++ )
    {
        // 1. Speicher aufteilen
        int previousRowOffset = ( i - 1 ) * params.nx;
        sendScatterPrevRow( waveData, prevRow, 
                            previousRowOffset, elementsPerProcess );
        // 2. Wellengleichung anwenden
        calcCurPartition( curRow, prevRow, prevPrevRow, 
                          proc, elemtsWithDeps );
        // 3. Partitionen im Master zusammenführen
        int currentRowOffset = i * params.nx;
        gather( waveData, curRow, 
                elementsPerProcess, currentRowOffset );        
        // im nächsten Durchlauf wird prev zu prev-prev
        // der Speicher von prevRow wird neu befüllt
        swap( prevPrevRow, prevRow );
        // 4. Synchronisierung
        MPI_Barrier( MPI_COMM_WORLD );
    }

    if ( isRoot() )
    {
        auto endTime = chrono::high_resolution_clock::now();
        auto duration = chrono::duration< double, milli > ( endTime - startTime ).count();
        cout << "OpenMPIStrategy " << comm_size <<  " "
             << params.nt << " " << params.nx << " " << duration << endl;
    }

    delete[] curRow;
    delete[] prevRow;
    delete[] prevPrevRow;
    delete[] waveData;

    MPI_Finalize();
}