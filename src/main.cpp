#include <iostream>
#include "maze.h"
#include <chrono>
#include <thread>

int main( int argc, char *argv[] )
{
    if( argc < 2 )
    {
        std::cout << ">>> Error reading the file. You must pass a valid file name as argument!\n";
        return EXIT_FAILURE;
    }

    Maze maze;
    maze.get_board( argv[1] );    
    auto curr_position = maze.get_start_position();

    #define ANIMATED
    
    while( not maze.is_outside( curr_position ) ){
        curr_position = maze.advance_one();
        #ifdef ANIMATED
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::cout << "\n\n";
            std::cout << maze;
        #endif
    }

    std::cout << ">>> You've reached the end of maze!\n";
    std::cout << ">>> The following way has " << maze.get_steps() << " steps from start to finish.\n";
    #ifndef ANIMATED
        std::cout << maze;
    #endif

    return EXIT_SUCCESS;
}