#ifndef TILLGRAPH
#define TILLGRAPH

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <algorithm>
#include <ctime>
#include <cfloat>
#include <numeric>
#include <climits>
using namespace std;


struct GraphEdge
{
	int v;
	int ts;
	GraphEdge *last, *next;
};


class TILL_Graph
{
public:
	// the number of vertices and the number of edges
	int max_v, max_e; // set them in "graph.cpp"
	int n, m;

	bool directed; // directed graph or undirected graph

	// graph and reversed graph
	GraphEdge **graph_head, **graph_r_head;
	GraphEdge *graph_edge, *graph_r_edge;

	// in-degree and out-degree
	int *d_in, *d_out;


	TILL_Graph(char *graph_file, bool directed);
	~TILL_Graph();

	void insert_edge(int u, int v, int ts);
	void insert_edge(GraphEdge **head, GraphEdge *edge, int i, int j, int ts, int edge_id);
	
};

#endif