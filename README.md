# Campus Navigator
This project is an implementation of a navigational system for the University of Illinois, Chicago, using data from OpenStreetMap. The program represents the university's paths and buildings as a graph where the vertices are nodes (points on a map) and edges represent paths between these points. The user can input a starting point and a destination building, and the program will use Dijkstra's algorithm to find the shortest path.

<img width="1144" alt="Screen Shot 2023-05-20 at 1 17 00 AM" src="https://github.com/abhati27/Campus-Navigator/assets/60200047/4bc93b80-a091-48f6-b14b-f1c0e022da46">

## File Structure
* `main.cpp`: This is the main driver code for the project. It reads data from the OpenStreetMap, constructs a graph representing the University of Illinois campus, and performs pathfinding using Dijkstra's algorithm.
* `tinyxml2.h`: This is a C++ XML Parser library for reading in and manipulating XML data. The project uses it to parse the OpenStreetMap data.
* `dist.h`: This header file contains the definition of the distBetween2Points function, which calculates the distance between two geographical coordinates.
* `osm.h`: This header file defines data structures for storing information about the map, buildings, footpaths, and their geographical coordinates.
* `graph.h`: This header file defines a generic graph data structure used to store the campus map as a graph.

## Usage
Compile the project and run the main.cpp using the makefile.

1. `make build` to compile the files and link them.
2. `make run` to run the compiled executable.

 You will be prompted to enter the OpenStreetMap filename. You can either enter the filename or hit Enter to use the default filename ("map.osm"). If the file is not found, an error message is printed, and the program exits.

Once the file is loaded successfully, the program reads the map nodes, footways, and university buildings from the OpenStreetMap data and creates a graph.

The user is then asked to enter a starting point and a destination point. These can be partial names or abbreviations of buildings at the University of Illinois. If either of these buildings is not found, the program prints a message indicating that the building was not found.

Once the buildings are successfully entered, the program finds the nearest nodes (points on the map) to these buildings and uses Dijkstra's algorithm to find the shortest path from the starting point to the destination. The program then prints out the distance and the path taken.


## Requirements
1. A C++ compiler , I used c++11 for this program
2. the `tinyxml` library
