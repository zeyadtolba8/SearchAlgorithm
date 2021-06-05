#include "Graph.h"


Graph:: Graph(int V)
{
	this->V = V;
	adj = new list<int>[V];
}

void Graph::addEdge(int v, int u)
{
	adj[v].push_back(u); // Add w to v’s list. 
}


int Graph::getV()
{
	return V;

}
	


Graph Graph::get_transpose()
{
	Graph graph_transpose(V);
	for (int v = 0; v < V; v++)
	{
		//Travers through all vertices adjacent to this vertex
		list<int>::iterator i;
		for (i = adj[v].begin(); i != adj[v].end(); ++i)
		{
			graph_transpose.adj[*i].push_back(v);

		}
	}
	return graph_transpose;
}


vector<int> Graph::Degree()
{
	vector<int>degree_vector(V, 0);

	for (int v = 0; v < V; v++)
	{
		//Travers through all vertices adjacent to this vertex
		list<int>::iterator i;
		for (i = adj[v].begin(); i != adj[v].end(); ++i)
		{
			degree_vector[v]++;

		}
	}
	return degree_vector;
}

int** Graph::display_adj()
{
	int** adj_of_vertices = new int* [V];
	for (int v = 0; v < V; ++v)
	{
		adj_of_vertices[v] = new int[V];
		int outdegree = 0;
		for (auto x : adj[v])
		{

			adj_of_vertices[v][outdegree] = x;
			outdegree++;
		}
			
	}

	for (int i = 0; i < V; i++)
	{
		for (int j = 0; j < V; j++)
		{
			if (adj_of_vertices[i][j] < 0)
				adj_of_vertices[i][j] = -1;
		}
	}
	return adj_of_vertices;
}

