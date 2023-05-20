/*main.cpp*/

//
// Anmol Vijay Bhatia
// U. of Illinois, Chicago
// CS 251: Spring 2020
// Project #07: open street maps, graphs, and Dijkstra's algorithm
// 
// References:
// TinyXML: https://github.com/leethomason/tinyxml2
// OpenStreetMap: https://www.openstreetmap.org
// OpenStreetMap docs:  
//   https://wiki.openstreetmap.org/wiki/Main_Page
//   https://wiki.openstreetmap.org/wiki/Map_Features
//   https://wiki.openstreetmap.org/wiki/Node
//   https://wiki.openstreetmap.org/wiki/Way
//   https://wiki.openstreetmap.org/wiki/Relation
//

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <string>
#include <limits>
#include <queue>
#include <algorithm>
#include <stack>

#include "tinyxml2.h"
#include "dist.h"
#include "osm.h"
#include "graph.h"

using namespace std;
using namespace tinyxml2;

const double INF = numeric_limits<double>::max();

using PairType = std::pair<long long, double>;

class Prioritize {
public:
    bool operator() (const PairType& p1, const PairType& p2) const {
        if (p1.second == p2.second) {
            return p1.first > p2.first;
        }
        return p1.second > p2.second;
    }
};


vector<long long> Dijkstra(graph<long long, double>& inputGraph, 
  long long startVertex, 
  map<long long, double>& shortestDistances,
  map<long long, long long>& predecessors)
{
  vector<long long>  visitedVertices;
  vector<long long> allVertices = inputGraph.getVertices();
  priority_queue<pair<long long, double>, vector<pair<long long, double>>, Prioritize> unvisitedVerticesQueue;
  set<long long> neighboringVertices;
  double edgeWeight, alternativeDistance;
  
  // Initialize every vertex as unvisited, with an infinite distance
  for(long long vertex : allVertices){
     unvisitedVerticesQueue.push(make_pair(vertex, INF));
     shortestDistances[vertex] = INF;
     predecessors[vertex] = 0;
  }

  long long currentVertex;
  
  // Start from the startVertex
  unvisitedVerticesQueue.push(make_pair(startVertex, 0));
  shortestDistances[startVertex] = 0;
  
  while(!unvisitedVerticesQueue.empty()){
     currentVertex = unvisitedVerticesQueue.top().first;
     unvisitedVerticesQueue.pop();
     
     if(shortestDistances[currentVertex] == INF){
        break;
     }
     else if(count(visitedVertices.begin(), visitedVertices.end(), currentVertex)){
        continue;
     }
     else {
        visitedVertices.push_back(currentVertex);
     }
     
     // Check all the neighbors of the current vertex
     neighboringVertices = inputGraph.neighbors(currentVertex);
     
     for(long long adjacentVertex : neighboringVertices){
        inputGraph.getWeight(currentVertex, adjacentVertex, edgeWeight);
        alternativeDistance = shortestDistances.at(currentVertex) + edgeWeight;
        
        // If a shorter path is found, update shortestDistances and predecessors
        if(alternativeDistance < shortestDistances.at(adjacentVertex)){
           shortestDistances[adjacentVertex] = alternativeDistance;
           unvisitedVerticesQueue.push(make_pair(adjacentVertex, alternativeDistance));
		   predecessors[adjacentVertex] = currentVertex;
        }
     }
  }
  return visitedVertices;
}


int main()
{
  map<long long, Coordinates>  nodeCoordinatesMap;     // Maps a Node ID to its coordinates (lat, lon)
  vector<FootwayInfo>          footwayInfoVector;  // Information about each footway, in no particular order
  vector<BuildingInfo>         buildingInfoVector; // Information about each building, in no particular order
  XMLDocument                  xmlMapDocument;
  graph <long long, double>    navigationGraph; // Graph where vertices are nodes (points on map) and edges are paths between them

  cout << "** Navigating UIC open street map **" << endl;
  cout << endl;
  cout << std::setprecision(8);

  string defaultFileName = "map.osm";
  string mapFileName;

  cout << "Enter map filename> ";
  getline(cin, mapFileName);

  if (mapFileName == "")
  {
    mapFileName = defaultFileName;
  }

  if (!LoadOpenStreetMap(mapFileName, xmlMapDocument))
  {
    cout << "**Error: unable to load open street map." << endl;
    cout << endl;
    return 0;
  }
  
  int nodeCount = ReadMapNodes(xmlMapDocument, nodeCoordinatesMap);
	
  for(auto node: nodeCoordinatesMap){
    navigationGraph.addVertex(node.first);
  }
	
  int footwayCount = ReadFootways(xmlMapDocument, footwayInfoVector);
	
  for(auto footway: footwayInfoVector){
    for(unsigned int i=0; i<footway.Nodes.size()-1; i++){
      navigationGraph.addEdge(footway.Nodes[i], footway.Nodes[i+1], distBetween2Points(nodeCoordinatesMap[footway.Nodes[i]].Lat, nodeCoordinatesMap[footway.Nodes[i]].Lon, nodeCoordinatesMap[footway.Nodes[i+1]].Lat, nodeCoordinatesMap[footway.Nodes[i+1]].Lon));
      navigationGraph.addEdge(footway.Nodes[i+1], footway.Nodes[i], distBetween2Points(nodeCoordinatesMap[footway.Nodes[i]].Lat, nodeCoordinatesMap[footway.Nodes[i]].Lon, nodeCoordinatesMap[footway.Nodes[i+1]].Lat, nodeCoordinatesMap[footway.Nodes[i+1]].Lon));
    }
  }


  int buildingCount = ReadUniversityBuildings(xmlMapDocument, nodeCoordinatesMap, buildingInfoVector);

  assert((unsigned)nodeCount == nodeCoordinatesMap.size());
  assert((unsigned)footwayCount == footwayInfoVector.size());
  assert((unsigned)buildingCount == buildingInfoVector.size());

  cout << endl;
  cout << "# of nodes: " << nodeCoordinatesMap.size() << endl;
  cout << "# of footways: " << footwayInfoVector.size() << endl;
  cout << "# of buildings: " << buildingInfoVector.size() << endl;
  cout << "# of vertices: " << navigationGraph.NumVertices() << endl;
  cout << "# of edges: " << navigationGraph.NumEdges() << endl;
  cout << endl;

  string startBuildingName, destinationBuildingName;
  BuildingInfo startBuildingInfo, destinationBuildingInfo; // For storing the start and destination building information
  
  cout << "Enter start (partial name or abbreviation), or #> ";
  getline(cin, startBuildingName);

  while (startBuildingName != "#")
{
    cout << "Enter destination (partial name or abbreviation)> ";
    getline(cin, destinationBuildingName);
    int isStartBuildingFound = 0;

    for(auto building: buildingInfoVector){
        if(startBuildingName == building.Abbrev){
            startBuildingInfo = building;
            isStartBuildingFound = 1;
            break;
        }
    }
    if(isStartBuildingFound == 0){
        for(auto building: buildingInfoVector){
            if(building.Fullname.find(startBuildingName) != std::string::npos){
                startBuildingInfo = building;
                isStartBuildingFound = 1;
                break;
            }
        }
    }
      
    if(isStartBuildingFound == 0){
        cout << "Start building not found" << endl;
    } else {
        int isDestinationBuildingFound = 0;
        for(auto building: buildingInfoVector){
            if(destinationBuildingName == building.Abbrev){
                destinationBuildingInfo = building;
                isDestinationBuildingFound = 1;
                break;
            }
        }
        if(isDestinationBuildingFound == 0){
            for(auto building: buildingInfoVector){
                if(building.Fullname.find(destinationBuildingName) != std::string::npos){
                    destinationBuildingInfo = building;
                    isDestinationBuildingFound = 1;
                    break;
                }
            }
        }
        if(isDestinationBuildingFound == 0){ 
            cout << "Destination building not found" << endl;
        } else {

    cout << "Starting point:" << endl; 
    cout << " " << startBuildingInfo.Fullname << endl;
    cout << " (" << startBuildingInfo.Coords.Lat << ", " << startBuildingInfo.Coords.Lon << ")" << endl;
    cout << "Destination point:" << endl;
    cout << " " << destinationBuildingInfo.Fullname << endl;
    cout << " (" << destinationBuildingInfo.Coords.Lat << ", " << destinationBuildingInfo.Coords.Lon << ")" << endl;

    long long nearestStartNodeId, nearestDestNodeId;
    double minDistanceToStart = INF;
    double minDistanceToDestination = INF;

    for(auto footway: footwayInfoVector){
        for(auto nodeId: footway.Nodes){
            if(distBetween2Points(startBuildingInfo.Coords.Lat, startBuildingInfo.Coords.Lon, nodeCoordinatesMap[nodeId].Lat, nodeCoordinatesMap[nodeId].Lon) < minDistanceToStart){
                nearestStartNodeId = nodeId;
                minDistanceToStart = distBetween2Points(startBuildingInfo.Coords.Lat, startBuildingInfo.Coords.Lon, nodeCoordinatesMap[nodeId].Lat, nodeCoordinatesMap[nodeId].Lon);
            }
            if(distBetween2Points(destinationBuildingInfo.Coords.Lat, destinationBuildingInfo.Coords.Lon, nodeCoordinatesMap[nodeId].Lat, nodeCoordinatesMap[nodeId].Lon) < minDistanceToDestination){
                nearestDestNodeId = nodeId;
                minDistanceToDestination = distBetween2Points(destinationBuildingInfo.Coords.Lat, destinationBuildingInfo.Coords.Lon, nodeCoordinatesMap[nodeId].Lat, nodeCoordinatesMap[nodeId].Lon);
            }
        }
    }


    cout << endl;
    cout << "Nearest start node:" << endl;
    cout << " " << nearestStartNodeId << endl;
    cout << " (" << nodeCoordinatesMap[nearestStartNodeId].Lat << ", " << nodeCoordinatesMap[nearestStartNodeId].Lon << ")" << endl;
    cout << "Nearest destination node:" << endl;
    cout << " " << nearestDestNodeId << endl;
    cout << " (" << nodeCoordinatesMap[nearestDestNodeId].Lat << ", " << nodeCoordinatesMap[nearestDestNodeId].Lon << ")" << endl << endl;
  
    cout << "Navigating with Dijkstra..." << endl;
    map<long long, double> shortestDistances;
    map<long long, long long> shortestPaths;
    Dijkstra(navigationGraph, nearestStartNodeId, shortestDistances, shortestPaths);

    long long currentVertex = nearestDestNodeId;
    stack<long long> pathStack;

    if(shortestPaths[currentVertex] == 0 && nearestStartNodeId != nearestDestNodeId){
        cout << "Sorry, destination unreachable" << endl;
    } else {
        cout << "Distance to destination: " << shortestDistances[nearestDestNodeId] << " miles" << endl;
        while(shortestPaths[currentVertex] != 0){
            pathStack.push(shortestPaths[currentVertex]);
            currentVertex = shortestPaths[currentVertex];
        }

        cout << "Path: ";
        while(!pathStack.empty()){
            cout << pathStack.top() << "->";
            pathStack.pop();
        }
        cout << nearestDestNodeId << endl;
    }
}
    }
    cout << endl;
    cout << "Enter start (partial name or abbreviation), or #> ";
    getline(cin, startBuildingName);
}


  cout << "** Done **" << endl;

  return 0;
}
