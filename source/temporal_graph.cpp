#include "temporal_graph.h"

int TemporalGraph::find(int u) {

    if (L[u] != u) {
        L[u] = find(L[u]);
    }

    return L[u];

}

int TemporalGraph::numOfVertices() {

    return n;

}

int TemporalGraph::numOfEdges() {

    return m;

}

TemporalGraph::Edge * TemporalGraph::getHeadEdge(int u) {

    return head_edge[u];

}

TemporalGraph::Edge * TemporalGraph::getNextEdge(Edge * e) {

    return e->next;

}

int TemporalGraph::getDestination(Edge * e) {

    return e->to;

}

int TemporalGraph::getInteractionTime(Edge * e) {

    return e->interaction_time;

}

void TemporalGraph::addEdge(int u, int v, int t) {

    if (head_edge[u]) {
        head_edge[u] = new Edge(v, t, head_edge[u]);
    }
    else {
        head_edge[u] = new Edge(v, t, nullptr);
    }

}

void TemporalGraph::shrink_to_fit() {

    L = new int[n];
    std::vector<std::pair<int, int>> temp;

    for (int t = 0; t <= tmax; ++t) {
        for (int u = 0; u < n; ++u) {
            L[u] = u;
        }
        std::vector<std::pair<int, int>>::iterator it;
        for (it = temporal_edge[t].begin(); it != temporal_edge[t].end(); it++) {
            int u = it->first;
            int v = it->second;
            if (find(u) == find(v)) {
                continue;
            }
            L[L[v]] = L[u];
            temp.push_back(std::make_pair(u, v));
        }
        temporal_edge[t].clear();
        for (it = temp.begin(); it != temp.end(); it++) {
            temporal_edge[t].push_back(std::make_pair(it->first, it->second));
        }
        temp.clear();
    }

    delete [] L;

}

TemporalGraph::TemporalGraph(char *graph_file, char *graph_type) {

    int u, v, t;
    std::ifstream fin(graph_file);

    is_directed = std::strcmp(graph_type, "Directed") == 0;
    is_general = 1;

    while (fin >> u >> v >> t) {
        n = std::max(n, std::max(u, v));
        ++m;
        tmax = std::max(tmax, t);
        while (temporal_edge.size() < t + 1) {
            temporal_edge.push_back(std::vector<std::pair<int, int>>());
        }
        temporal_edge[t].push_back(std::make_pair(u, v));
    }
    ++n;
    
}

TemporalGraph::TemporalGraph(TemporalGraph * Graph, int ts, int te) {

    n = Graph->numOfVertices();
    m = Graph->numOfEdges();
    tmax = Graph->tmax;
    is_directed = Graph->is_directed;
    is_general = 0;

    while (head_edge.size() < n) {
        head_edge.push_back(nullptr);
    }

    std::vector<std::pair<int, int>>::iterator it;

    for (int t = ts; t <= te; ++t) {
        for (it = Graph->temporal_edge[t].begin(); it != Graph->temporal_edge[t].end(); it++) {
            int u = it->first;
            int v = it->second;
            addEdge(u, v, t);
            if (!is_directed) {
                addEdge(v, u, t);
            }
        }
    }
    
}

TemporalGraph::~TemporalGraph() {

    std::vector<Edge *>::iterator it;

    for (it = head_edge.begin(); it != head_edge.end(); it++) {
        Edge * temp = *it;
        while (temp) {
            Edge * next = temp->next;
            delete temp;
            temp = next;
        }
    }

}