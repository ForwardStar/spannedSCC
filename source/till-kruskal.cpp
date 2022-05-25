#include "till-kruskal.h"

int TILL_Kruskal::find(int ts, int u) {
    
    if (L[ts][u] != u) {
        return find(ts, L[ts][u]);
    }

    return u;

}

void TILL_Kruskal::unioN(int ts, int u, int v, int t) {

    int mount_u = find(ts, u);
    int mount_v = find(ts, v);

    // Already in the same connected component.
    if (mount_u == mount_v) {
        return;
    }

    // Merge the smaller connected component into the larger one.
    if (size[mount_u] < size[mount_v]) {
        std::swap(u, v);
        std::swap(mount_u, mount_v);
    }
    size[mount_u] += size[mount_v];
    
    L[ts][mount_v] = mount_u;
    T[ts][mount_v] = t;

}

void TILL_Kruskal::update(int u, int v, int ts, int te, TILL * TILL_Index) {

    if (find(ts, u) == find(ts, v) || (!TILL_Index->span_reach(v, u, ts, te, true))) {
        return;
    }

    unioN(ts, u, v, te);

    std::vector<int> vertices_to_be_erased;

    std::unordered_set<int>::iterator it;
    for (it = isolated_vertices.begin(); it != isolated_vertices.end(); it++) {
        if (find(ts, u) != find(ts, *it) && TILL_Index->span_reach(v, *it, ts, te, true) && TILL_Index->span_reach(*it, u, ts, te, true)) {
            unioN(ts, u, *it, te);
            vertices_to_be_erased.push_back(*it);
        }
    }

    std::vector<int>::iterator it1;
    for (it1 = vertices_to_be_erased.begin(); it1 != vertices_to_be_erased.end(); it1++) {
        isolated_vertices.erase(*it1);
    }

    /*
    std::unordered_set<int> visited_vertices;

    std::queue<int> Q;
    Q.push(v);
    visited_vertices.insert(v);

    while (!Q.empty()) {
        int now = Q.front();
        Q.pop();
        std::vector<Triplet>::iterator it;
        for (it = TILL_Index->out_label[TILL_Index->order[now]].begin(); it != TILL_Index->out_label[TILL_Index->order[now]].end(); it++) {
            if (TILL_Index->span_reach(v, TILL_Index->idx[it->v], ts, te, true) && TILL_Index->span_reach(TILL_Index->idx[it->v], u, ts, te, true)) {
                if (find(ts, u) != find(ts, TILL_Index->idx[it->v])) {
                    unioN(ts, u, TILL_Index->idx[it->v], te);
                    Q.push(TILL_Index->idx[it->v]);
                    visited_vertices.insert(TILL_Index->idx[it->v]);
                    continue;
                }
                if (visited_vertices.find(TILL_Index->idx[it->v]) == visited_vertices.end()) {
                    Q.push(TILL_Index->idx[it->v]);
                    visited_vertices.insert(TILL_Index->idx[it->v]);
                }
            }
        }
    }

    visited_vertices.clear();
    Q.push(u);
    visited_vertices.insert(u);

    while (!Q.empty()) {
        int now = Q.front();
        Q.pop();
        std::vector<Triplet>::iterator it;
        for (it = TILL_Index->in_label[TILL_Index->order[now]].begin(); it != TILL_Index->in_label[TILL_Index->order[now]].end(); it++) {
            if (TILL_Index->span_reach(v, TILL_Index->idx[it->v], ts, te, true) && TILL_Index->span_reach(TILL_Index->idx[it->v], u, ts, te, true)) {
                if (find(ts, u) != find(ts, TILL_Index->idx[it->v])) {
                    unioN(ts, u, TILL_Index->idx[it->v], te);
                    Q.push(TILL_Index->idx[it->v]);
                    visited_vertices.insert(TILL_Index->idx[it->v]);
                    continue;
                }
                if (visited_vertices.find(TILL_Index->idx[it->v]) == visited_vertices.end()) {
                    Q.push(TILL_Index->idx[it->v]);
                    visited_vertices.insert(TILL_Index->idx[it->v]);
                }
            }
        }
    }
    */

}

std::stringstream TILL_Kruskal::solve(int n, int ts, int te) {
    
    std::stringstream Ans;
    int *Vis = new int[n];
    std::vector<int> *CurrentCC = new std::vector<int>[n]();

    Ans << "The spanned strongly connected components in [" << ts << ", " << te << "] are:\n";
    for (int u = 0; u < n; ++u) {
        Vis[u] = -1;
    }

    for (int u = 0; u < n; ++u) {
        if (Vis[u] == -1) {
            Vis[u] = u;
            CurrentCC[u].push_back(u);

            int current = u;
            while (L[ts][current] != current && T[ts][current] <= te) {
                current = L[ts][current];
                if (Vis[current] != -1) {
                    std::vector<int>::iterator it;
                    for (it = CurrentCC[u].begin(); it != CurrentCC[u].end(); it++) {
                        Vis[*it] = Vis[current];
                        CurrentCC[Vis[current]].push_back(*it);
                    }
                    CurrentCC[u].clear();
                    break;
                }
                CurrentCC[u].push_back(current);
                Vis[current] = u;
            }
        }
    }

    for (int u = 0; u < n; ++u) {
        if (CurrentCC[u].size() == 0) {
            continue;
        }
        std::sort(CurrentCC[u].begin(), CurrentCC[u].end());
        std::vector<int>::iterator it;
        Ans << "{ ";
        for (it = CurrentCC[u].begin(); it != CurrentCC[u].end(); it++) {
            Ans << *it << " ";
        }
        Ans << "}\n";
    }

    delete [] Vis;
    delete [] CurrentCC;

    return Ans;

}

TILL_Kruskal::TILL_Kruskal(TemporalGraph * Graph, TILL * TILL_Index) {
    
    int start_time = time(NULL);
    start_time = time(NULL);

    n = Graph->numOfVertices();
    m = Graph->numOfEdges();
    tmax = Graph->tmax;
    L = new int *[tmax + 1];
    T = new int *[tmax + 1];
    size = new int[n];

    for (int ts = 0; ts <= tmax; ++ts) {
        L[ts] = new int[n];
        T[ts] = new int[n];
        for (int u = 0; u < n; ++u) {
            L[ts][u] = u;
            T[ts][u] = ts;
        }
    }
    
    for (int ts = 0; ts <= tmax; ++ts) {
        for (int u = 0; u < n; ++u) {
            size[u] = 1;
            isolated_vertices.insert(u);
        }
        for (int te = ts; te <= tmax; ++te) {
            std::vector<std::pair<int, int>>::iterator it;
            for (it = Graph->temporal_edge[te].begin(); it != Graph->temporal_edge[te].end(); it++) {
                update(it->first, it->second, ts, te, TILL_Index);
            }
        }
        isolated_vertices.clear();
        putProcess(double(ts) / tmax, difftime(time(NULL), start_time));
    }

    delete [] size;

}

TILL_Kruskal::~TILL_Kruskal() {

    for (int ts = 0; ts <= tmax; ++ts) {
        delete [] L[ts];
        delete [] T[ts];
    }

    delete [] L;
    delete [] T;

}

void TILL_kruskal(TILL_Kruskal * Index, int vertex_num, char * query_file, char * output_file) {

    int ts, te;
    int query_num = 0;
    std::ifstream fin(query_file);
    std::ofstream fout(output_file);

    while (fin >> ts >> te) {
        ++query_num;
    }

    fin = std::ifstream(query_file);

    int i = 0;
    int start_time = time(NULL);
    while (fin >> ts >> te) {
        fout << Index->solve(vertex_num, ts, te).str() << std::endl;
        putProcess(double(++i) / query_num, difftime(time(NULL), start_time));
    }

    std::cout << "Average (per query): " << timeFormatting(difftime(time(NULL), start_time) / query_num).str() << std::endl;

}