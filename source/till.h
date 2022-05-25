#ifndef TILLINDEX
#define TILLINDEX

#include "till_graph.h"
#include "commonfunctions.h"


struct Triplet // triplet in TILL-Index
{
	int v;
	int t1, t2;
	Triplet(int _v, int _t1, int _t2): v(_v), t1(_t1), t2(_t2) {};
	bool operator< (const Triplet &x) const
	{
		return v < x.v;
	}
};


struct Tri // triplet in priority queue Q
{
	int v;
	int t1, t2;
	Tri(int _v, int _t1, int _t2): v(_v), t1(_t1), t2(_t2) {};
	bool operator< (const Tri &x) const
	{
		return t2 - t1 > x.t2 - x.t1 || (t2 - t1 == x.t2 - x.t1 && v > x.v);
	}
};


class TILL
{
private:
	// the number of vertices and the number of edges
	int max_v, max_e; // set them in "graph.cpp"
	int n, m;

	bool directed; // directed graph or undirected graph

	// graph and reversed graph
	GraphEdge **graph_head, **graph_r_head;
	GraphEdge *graph_edge, *graph_r_edge;

	// in-degree and out-degree
	int *d_in, *d_out;

	int theta; // set it in "till.cpp"

	priority_queue<Tri> Q;


	void construct_for_a_vertex(GraphEdge **head, vector<Triplet> *lable, int u, bool in);


public:
	// TILL-Index
	vector<Triplet> *in_label, *out_label;

	int *idx; // order --> ID
	int *order; // ID --> order
	
	TILL(TILL_Graph *graph);
	~TILL();

	void construct();
	bool span_reach(int u, int v, int t1, int t2, bool original_id = false);
};

#endif