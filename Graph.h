#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include<algorithm>
#include <stack>
#include <list>
#include <set>
using namespace std;


#pragma once
class Graph
	{
		int V; // No. of vertices 

		// Pointer to an array containing 
		// adjacency lists 
		list<int>* adj;

	

	public:

		int getV();

		Graph(int V); // Constructor 

		// function to add an edge to graph 
		void addEdge(int v, int u);

		// function that return the transpose of a given graph
		Graph get_transpose();

		vector<int>Degree();

		int** display_adj();
		
	};


