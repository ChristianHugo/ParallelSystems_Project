#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>

#include "gui/open_gl_gui.h"

using namespace std;

namespace
{
    double *waveData = nullptr;
    int timespteps = -1, positions = -1;
    double posDiv;

    int flatIndex( int t, int x )
    {
        return t * positions + x;
    }

    void displayFunction( int timestep )
    {

        glClear( GL_COLOR_BUFFER_BIT );

        glPushMatrix();
        glTranslatef( -1.0, 0.0, 0.0 );
        
        glScalef( 1.0, 1000.0, 1.0 );
        
        glBegin( GL_LINE_STRIP );
        for ( int i = 0; i < positions; i++ )
            glVertex2f( posDiv * i, waveData[ flatIndex( timestep, i ) ]  ) ;
        glEnd();
        glFlush();
        glPopMatrix(); 

        if ( ++timestep < timespteps )
            glutTimerFunc( 100, displayFunction, ++timestep );
    }

    void init( int argc, char **argv )
    {
        glutInit( &argc, argv );
    
        glPointSize( 8.0f );
        glColor3f( 1.0f, 0.0f, 0.0f );

        glutInitWindowSize( 400, 400 );
        glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );

        glutTimerFunc( 0, displayFunction, 0 );
        glutCreateWindow( "wave gui" );
    }
}

/*!
    \brief initalisiert die GUI

    erwartet die Kommandozeilenparameter und reicht sie an glut weiter,
    erwartet die Werte der Welle

    \note Die GUI wird initialisiert nachdem die Daten berechnet wurden
    und nicht währenddessen aktualisiert.
    

    \param argc Länge der Kommandozeilenparameter
    \param argv array der Kommandozeilenparameter als char *
    \param data Matrix M mit den Werten der Welle
    \param nt Anzahl diekrete Zeitschritte
    \param nx ANzah diskrete Positionen
 */ 
void open_gl_gui_n::initGUI( int argc, char **argv, 
                             double *data, int nt, int nx )
{
    waveData = data;
    timespteps = nt;
    positions = nx;

    posDiv = 2.0 / positions;

    init( argc, argv );

    glutMainLoop();
}