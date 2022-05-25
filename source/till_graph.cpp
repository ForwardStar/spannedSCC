#include "till_graph.h"


TILL_Graph::TILL_Graph(char *graph_file, bool _directed)
{
	max_v = 10000000;
	max_e = 100000000;
	
	n = 0;
	m = 0;

	directed = _directed;
	
	graph_head = new GraphEdge* [max_v];
	graph_edge = new GraphEdge [max_e + 1];
	memset(graph_head, 0, sizeof(*graph_head) * max_v);
	
	if (directed)
	{
		graph_r_head = new GraphEdge* [max_v];
		graph_r_edge = new GraphEdge [max_e + 1];
		memset(graph_r_head, 0, sizeof(*graph_r_head) * max_v);
	}
	else
	{
		graph_r_head = graph_head;
		graph_r_edge = graph_edge;
	}

	d_in = new int [max_v];
	memset(d_in, 0, sizeof(*d_in) * max_v);

	if (directed)
	{
		d_out = new int [max_v];
		memset(d_out, 0, sizeof(*d_out) * max_v);
	}
	else
		d_out = d_in;

	int u, v;
	int ts;
	
	ifstream fin(graph_file);
	while (fin >> u >> v >> ts)
	{
		n = max(n, u + 1);
		n = max(n, v + 1);
		d_out[u]++;
		if (directed) d_in[v]++;
		insert_edge(u, v, ts);
	}
	fin.close();
}


TILL_Graph::~TILL_Graph()
{
	if (graph_head) delete[] graph_head;
	if (graph_edge) delete[] graph_edge;
	if (d_in) delete[] d_in;

	if (directed)
	{
		if (graph_r_head) delete[] graph_r_head;
		if (graph_r_edge) delete[] graph_r_edge;
		if (d_out) delete[] d_out;
	}
}


void TILL_Graph::insert_edge(GraphEdge **head, GraphEdge *edge, int i, int j, int ts, int edge_id)
{
	edge[edge_id].v = j;
	edge[edge_id].ts = ts;
	edge[edge_id].next = head[i];
	edge[edge_id].last = NULL;
	if (head[i]) head[i] -> last = &edge[edge_id];
	head[i] = &edge[edge_id];
}


void TILL_Graph::insert_edge(int u, int v, int ts)
{
	m++;
	if ((directed && m > max_e) || (!directed && m * 2 > max_e))
	{
		cout << "max_e is too small.\n";
		exit(0);
	}
	if (directed)
	{
		insert_edge(graph_head, graph_edge, u, v, ts, m);
		insert_edge(graph_r_head, graph_r_edge, v, u, ts, m);
	}
	else
	{
		insert_edge(graph_head, graph_edge, u, v, ts, m * 2 - 1);
		insert_edge(graph_head, graph_edge, v, u, ts, m * 2);
	}
}