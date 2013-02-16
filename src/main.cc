/* 
 * File:   Uno.cc
 * Author: Lucas
 *
 * Created on May 6, 2009, 1:06 PM
 */

#include "Uno.h"
#include <iostream>
using std::cout;
using std::cerr;
using std::cin;
using std::endl;

#include <stdexcept>
using std::exception;

#include <string>
using std::string;

#include <cstdlib>
using std::srand;
#include <ctime>
using std::time;


int main( int argc, char* argv[])
{
    srand( time(NULL) );
    try 
    {
        int human = 0;
        int ai = 0;
        if ( argc != 3 )
        {
            cerr << "Usage: uno <numHuman> <numAI>" << endl;
            exit(1);
        }
        human = atoi( argv[1] );
        ai = atoi( argv[2] );

        if ( human<0 || ai<0 )
        {
            cerr << "player counts cannot be negative" << endl;
            exit(1);
        }
        if ( human+ai < 2 )
        {
            cerr << "must have at least two players" << endl;
            exit(1);
        }
        // ctor takes #human players then #total players including AI
        Uno game = Uno(human, human+ai);

        // start the game!
        game.beginGame();

    }
    catch( invalid_argument& e )
    {
        cerr << "Exception Occured: " << e.what() << endl;
    }
    catch( logic_error& e )
    {
        cerr << "Exception Occured: " << e.what() << endl;
    }
    catch ( exception& e )
    {
        cerr << "Exception Occured: " << e.what() << endl;
    }
    catch(...)
    {
        cerr << "Unknown exception occurred.\n";
    }
    return 0;
}
