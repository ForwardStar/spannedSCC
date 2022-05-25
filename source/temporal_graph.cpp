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

    m = 0;

    if (!is_directed) {
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
            m += temp.size();
            for (it = temp.begin(); it != temp.end(); it++) {
                temporal_edge[t].push_back(std::make_pair(it->first, it->second));
            }
            temp.clear();
        }

        delete [] L;
    }
    else {
        std::unordered_set<int> *in_label = new std::unordered_set<int>[n];
        std::unordered_set<int> *out_label = new std::unordered_set<int>[n];
        std::unordered_set<int> *implied_in_label = new std::unordered_set<int>[n];
        std::unordered_set<int> *implied_out_label = new std::unordered_set<int>[n];
        int *idx = new int[n];

        int start_time = time(NULL);

        for (int u = 0; u < n; ++u) {
            implied_in_label[u].clear();
            implied_out_label[u].clear();
        }

        for (int t = 0; t <= tmax; ++t) {
            for (int u = 0; u < n; ++u) {
                in_label[u].clear();
                out_label[u].clear();
                implied_in_label[u].clear();
                implied_out_label[u].clear();
                idx[u] = u;
            }
            std::vector<std::pair<int, int>>::iterator it;
            for (it = temporal_edge[t].begin(); it != temporal_edge[t].end(); it++) {
                if (it->first == it->second) {
                    continue;
                }
                if (in_label[it->second].find(it->first) == in_label[it->second].end()) {
                    in_label[it->second].insert(it->first);
                    implied_in_label[it->second].insert(it->first);
                }
                if (out_label[it->first].find(it->second) == out_label[it->first].end()) {
                    out_label[it->first].insert(it->second);
                    implied_out_label[it->first].insert(it->second);
                }
            }
            std::sort(idx, idx + n, [in_label, out_label](int i, int j) {
                return in_label[i].size() * out_label[i].size() < in_label[j].size() * out_label[j].size();
            });
            for (int i = 0; i < n; ++i) {
                if (implied_in_label[idx[i]].size() * implied_out_label[idx[i]].size() > n) {
                    break;
                }
                std::unordered_set<int>::iterator it1, it2;
                for (it1 = implied_in_label[idx[i]].begin(); it1 != implied_in_label[idx[i]].end(); it1++) {
                    for (it2 = implied_out_label[idx[i]].begin(); it2 != implied_out_label[idx[i]].end(); it2++) {
                        if (implied_out_label[*it1].find(*it2) == implied_out_label[*it1].end()) {
                            implied_out_label[*it1].insert(*it2);
                            if (out_label[*it1].find(*it2) != out_label[*it1].end()) {
                                out_label[*it1].erase(*it2);
                            }
                        }
                        if (implied_in_label[*it2].find(*it1) == implied_in_label[*it2].end()) {
                            implied_in_label[*it2].insert(*it1);
                            if (in_label[*it2].find(*it1) != in_label[*it2].end()) {
                                in_label[*it2].erase(*it1);
                            }
                        }
                    }
                    implied_out_label[*it1].erase(idx[i]);
                }
                for (it1 = implied_out_label[idx[i]].begin(); it1 != implied_out_label[idx[i]].end(); it1++) {
                    implied_in_label[*it1].erase(idx[i]);
                }
            }
            temporal_edge[t].clear();
            for (int u = 0; u < n; ++u) {
                m += out_label[u].size();
                std::unordered_set<int>::iterator it;
                for (it = out_label[u].begin(); it != out_label[u].end(); it++) {
                    temporal_edge[t].push_back(std::pair<int, int>(u, *it));
                }
            }
            putProcess(double(t) / tmax, difftime(time(NULL), start_time));
        }

        delete [] in_label;
        delete [] out_label;
        delete [] implied_in_label;
        delete [] implied_out_label;
        delete [] idx;
    }

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