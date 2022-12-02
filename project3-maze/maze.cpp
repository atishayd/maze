/*
 maze.cpp
 */

#include <iostream>
#include "mazeio.h"
#include "queue.h"
//Including fstream in order to allow command line ifile args
#include "fstream"

using namespace std;

// Prototype for maze_search, which you will fill in below.
int maze_search(char** maze, int rows, int cols);
//Prototyping of structEqual, a function used to compare two structs
bool structEqual(Location a, Location b);

// Add other prototypes here for any functions you wish to use

//Definition of structEqual
bool structEqual(Location a, Location b)
{
    if ((a.row == b.row) && (a.col == b.col))
    {
        return true;
    }
    return false;
}
// main function to read, solve maze, and print result
int main(int argc, char* argv[]) {
    int rows, cols, result;
    char** mymaze=NULL;
    const char* invalid_char_message = "Error, input format incorrect.";
    //New char2 created in order to handle error of invalid character entry.
    const char* invalid_char2_message = "Error, invalid character.";
    const char* invalid_maze_message = "Invalid maze.";
    const char* no_path_message = "No path could be found!"; // No solution to maze
    
    //Check to see enoguh arguments were entered. If not, print an error.
    if(argc < 2)
    {
        cout << "Please provide a maze input file" << endl;
        return 1;
    }
    
    mymaze = read_maze(argv[1], &rows, &cols); // <---TASK: COMPLETE THIS FOR CHECKPOINT 1
    
    //Invalid input format check- output message.
    if (mymaze == NULL)
    {
        cout << invalid_char_message << endl;
    }
    // For checkpoint 2 you should check the validity of the maze
    // You may do so anywhere you please and can abstract that
    // operation with a function or however you like.
    
    //Invalid char present within maze such as '!' which isn't allowed.
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
          //Check if a symbol is present that isn't 'S', 'F', '.', '#'
            if((mymaze[i][j] != 'S') && (mymaze[i][j] != 'F') && (mymaze[i][j] != '.') && (mymaze[i][j] != '#'))
            {
                cout << invalid_char2_message << endl;
                //Dynamic deletion of maze
                for (int k = 0; k < rows; k++)
                {
                    delete[] mymaze[k];
                }
                delete[] mymaze;
                return 0;
            }
        }
    }
    //Calling the maze_search function
    result = maze_search(mymaze, rows, cols);
    //Error handling output message if maze is poorly formatted
    if (result == -1)
    {
        cout << invalid_maze_message << endl;
    } else {
        cout << rows << " " << cols << endl;
    }
    
    //If no path is found for the maze.
    if (result == 0)
    {
        cout << no_path_message << endl;
    }
    
    //================================
    // When working on Checkpoint 4, you will need to call maze_search
    // and output the appropriate message or, if successful, print
    // the maze.  But for Checkpoint 1, we print the maze, regardless.
    print_maze(mymaze, rows, cols);
    
    //================================
    // ADD CODE BELOW
    // to delete all memory that read_maze allocated: CHECKPOINT 2
  
    //Deallocation of maze
    for (int i = 0; i < rows; i++)
    {
        delete[] mymaze[i];
    }
    delete[] mymaze;
    
    return 0;
}

/**************************************************
 * Attempt to find shortest path and return:
 *  1 if successful
 *  0 if no path exists
 *
 * If path is found fill it in with '*' characters
 *  but don't overwrite the 'S' and 'F' cells
 * NOTE: don't forget to deallocate memory in here too!
 *************************************************/
int maze_search(char** maze, int rows, int cols)
{
    
    // *** You complete **** CHECKPOINT 4
    //Creating a new queue
    Queue queue(rows*cols);
    //Creating the predecessor array
    Location** predecessor = new Location*[rows];
    //Creating the predecessor columns as it is a 2d array
    for (int i = 0; i < rows; i++)
    {
        predecessor[i] = new Location[cols];
    }
    //Creating 2d array explored which is a bool**to see if we have already
    //been to a certain part of the maze.
    bool** explored = new bool*[rows];
    for (int i = 0; i < rows; i++)
    {
        explored[i] = new bool[cols];
    }

    //Setting every element in the maze to false initially
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            explored[i][j] = false;
        }
    }
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            predecessor[i][j].row = -1;
            predecessor[i][j].col = -1;
        }
    }
    
    //Initializing the startRow, startCol, startCount, endCount
    int startRow = 0;
    int startCol = 0;
    int startCount = 0;
    int endCount = 0;
    
    //Nested for loop that aims to find the start point and endpoint
    //If found, set a startRow, startCol, and make a count to see
    //if more than one start point exists (for a badly formatted maze)
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (maze[i][j] == 'S')
            {

                startRow = i;
                startCol = j;
                startCount++;
            }
            if (maze[i][j] == 'F')
            {
                endCount++;
            }
        }
    }
    if (startCount != 1 || endCount != 1)
    {
        return -1;
    }

    //Creating the start location
    Location start;
    //Setting data members
    start.row = startRow;
    start.col = startCol;
    
    //1 = true
    //mark the start location as explored
    explored[start.row][start.col] = 1;
    //Adding start to the back of queue
    queue.add_to_back(start);
    //Setting a current location
    Location current;
    
    //This while loop is the main logic, body of the whole program.
    //While the queue is NOT empty-
    while (!queue.is_empty())
    {
    
       //set loc = extract the item from the front of q
        current = queue.remove_from_front();
        
        //If the end is found, exit from while loop
        if(maze[current.row][current.col] == 'F')
        {
            break;
        }
        
        //Declaring the north, south, east, west of the current
        //This is done in order to find the neighbours for our BFS.
        Location currentNorth;
        currentNorth.row = current.row-1;
        currentNorth.col = current.col;
        Location currentEast;
        currentEast.row = current.row;
        currentEast.col = current.col+1;
        Location currentSouth;
        currentSouth.row = current.row+1;
        currentSouth.col = current.col;
        Location currentWest;
        currentWest.row = current.row;
        currentWest.col = current.col-1;
        
        //All checks from lines 237 to 272 are checking if the neighbor is 
        //within the maze, if not, reject input, if it is, add it to queue, 
        //set index as explored, set predecessor of neighbor = loc
        // for each neighbor (i.e., N,W,S, and E) of loc do:
        if ((currentNorth.row >= 0) && (currentNorth.row < rows))
        {
            if ((explored[currentNorth.row][currentNorth.col] == false) && (maze[currentNorth.row][currentNorth.col] != '#'))
            {
              // if the neighbor is valid, open, and not yet explored do:
              // add the neighbor to the back of q
                queue.add_to_back(currentNorth);
                //  mark the neighbor as explored
                explored[currentNorth.row][currentNorth.col] = true;
                //set predecessor of the neighbor = loc
                predecessor[currentNorth.row][currentNorth.col] = current;
            }
        }
        
        if ((currentWest.col >= 0) && (currentWest.col < cols) && (explored[currentWest.row][currentWest.col] == false) && (maze[currentWest.row][currentWest.col] != '#'))
        {
            queue.add_to_back(currentWest);
            explored[currentWest.row][currentWest.col] = true;
            predecessor[currentWest.row][currentWest.col] = current;
        }
        
        if ((currentSouth.row < rows) && ((currentSouth.row > 0)) && (explored[currentSouth.row][currentSouth.col] == false) && (maze[currentSouth.row][currentSouth.col] != '#'))
        {
            queue.add_to_back(currentSouth);
            explored[currentSouth.row][currentSouth.col] = true;
            predecessor[currentSouth.row][currentSouth.col] = current;
        }
        
        if ((currentEast.col < cols) && (currentEast.col > 0) && (explored[currentEast.row][currentEast.col] == false) && (maze[currentEast.row][currentEast.col] != '#'))
        {
            queue.add_to_back(currentEast);
            explored[currentEast.row][currentEast.col] = true;
            predecessor[currentEast.row][currentEast.col] = current;
        }
        explored[current.row][current.col] = true;
    }
    //Exiting loop, checking the condition again to see if its = F
    if (maze[current.row][current.col] == 'F')
    {
      //Utilising struct equal function to see
      //while current != start
        while (structEqual(current, start) == false)
        {
          //This check ensures that we don't overwrite 
          //F because it would produce an invalid output
          //where F would be a *
            if (maze[current.row][current.col] != 'F')
            {
              //Replace with *
                maze[current.row][current.col] = '*';
            }
            //Set as predecessor
            current = predecessor[current.row][current.col];
        }
        //Deallocation of memory
        for (int i=0; i < rows; i++)
        {
            delete [] predecessor[i];
            delete [] explored[i];
        }
        delete [] predecessor;
        delete [] explored;
        return 1;
    }
    //Deallocation of memory
    for (int i=0; i < rows; i++)
    {
        delete [] predecessor[i];
        delete [] explored[i];
    }
    delete [] predecessor;
    delete [] explored;
    return 0;
}