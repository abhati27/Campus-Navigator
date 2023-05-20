/*graph.h*/

//
// Aryann Chodankar
//
// Basic graph class using adjacency matrix representation.  Currently
// limited to a graph with at most 100 vertices.
//
// original author: Prof. Joe Hummel
// U. of Illinois, Chicago
// CS 251: Spring 2020
//

#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>
#include <set>
#include <map>
using namespace std;


template<typename VertexT, typename WeightT>
class graph
{
private:
  map<VertexT, WeightT> adjMap;
  map<VertexT, map<VertexT, WeightT>> graphMap;
  bool _LookupVertex(VertexT v) const
  {
	  if(graphMap.count(v)!=0){
		  return true;
	  }
	  return false;
  }

public:
  //
  // constructor:
  //
  // Constructs an empty graph where n is the max # of vertices 
  // you expect the graph to contain.
  //
  // NOTE: the graph is implemented using an adjacency matrix.
  // If n exceeds the dimensions of this matrix, an exception 
  // will be thrown to let you know that this implementation
  // will not suffice.
  //
  graph()
  {
  
  }

  //
  // NumVertices
  //
  // Returns the # of vertices currently in the graph.
  //
  int NumVertices() const
  {
	return graphMap.size();
  }

  //
  // NumEdges
  //
  // Returns the # of edges currently in the graph.
  //
  int NumEdges() const
  {
	int count=0;
	for(auto vertex: graphMap){
		count+=vertex.second.size();
	}
    return count;
  }

  //
  // addVertex
  //
  // Adds the vertex v to the graph if there's room, and if so
  // returns true.  If the graph is full, or the vertex already
  // exists in the graph, then false is returned.
  //
  bool addVertex(VertexT v)
  {
	if(_LookupVertex(v)){
		return false;
	}
	graphMap[v]=adjMap;
    return true;
  }

  //
  // addEdge
  //
  // Adds the edge (from, to, weight) to the graph, and returns
  // true.  If the vertices do not exist or for some reason the
  // graph is full, false is returned.
  //
  // NOTE: if the edge already exists, the existing edge weight
  // is overwritten with the new edge weight.
  //
  bool addEdge(VertexT from, VertexT to, WeightT weight)
  {
	if(!_LookupVertex(from)){
		return false;
	}
	if(!_LookupVertex(to)){
		return false;
	}
	graphMap[from][to]=weight;
    return true;
  }

  //
  // getWeight
  //
  // Returns the weight associated with a given edge.  If 
  // the edge exists, the weight is returned via the reference
  // parameter and true is returned.  If the edge does not 
  // exist, the weight parameter is unchanged and false is
  // returned.
  //
  bool getWeight(VertexT from, VertexT to, WeightT& weight) const
  {
	if(!_LookupVertex(from)){
		return false;
	}
	if(graphMap.at(from).count(to)==0){
		return false;
	}
	weight = graphMap.at(from).at(to);
    return true;
  }

  //
  // neighbors
  //
  // Returns a set containing the neighbors of v, i.e. all
  // vertices that can be reached from v along one edge.
  // Since a set is returned, the neighbors are returned in
  // sorted order; use foreach to iterate through the set.
  //
  set<VertexT> neighbors(VertexT v) const
  {
    set<VertexT>  S;
	if(!_LookupVertex(v)){
		return S;
	}
	for(auto vertex:graphMap.at(v)){
		VertexT dest=vertex.first;
		S.insert(dest);
	}

    return S;
  }

  //
  // getVertices
  //
  // Returns a vector containing all the vertices currently in
  // the graph.
  //
  vector<VertexT> getVertices() const
  {
	vector<VertexT> retVertices;
	for(auto vertex:graphMap){
		retVertices.push_back(vertex.first);
	}
    return retVertices;  // returns a copy:
  }

  //
  // dump
  // 
  // Dumps the internal state of the graph for debugging purposes.
  //
  // Example:
  //    graph<string,int>  G(26);
  //    ...
  //    G.dump(cout);  // dump to console
  //
  void dump(ostream& output) const
  {
    output << "***************************************************" << endl;
    output << "********************* GRAPH ***********************" << endl;

    output << "**Num vertices: " << this->NumVertices() << endl;
    output << "**Num edges: " << this->NumEdges() << endl;

    output << endl;
	for(auto vertex: graphMap){
		output<<" "<<vertex.first<<": ";
		for(auto edge: vertex.second){
			output<< " ("<<vertex.first<<","<<edge.first<<","<<edge.second<<") ";
		}
		output<<endl;
	}

    output << "**************************************************" << endl;
  }

};