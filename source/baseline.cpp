#include "baseline.h"

int BaselineIndex::find(int ts, int u) {
    
    if (L[ts][u] != u) {
        return find(ts, L[ts][u]);
    }

    return u;

}

void BaselineIndex::unioN(int ts, int u, int v, int t) {

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

void BaselineIndex::tarjan(int now, int &t, int &ts, int &te) {

    inOrder[now] = ++t;
    lowestOrder[now] = t;
    Vis[now] = true;
    Stack.push(now);

    std::vector<int> to_delete;

    std::unordered_set<int>::iterator it;
    for (it = outLabel[now].begin(); it != outLabel[now].end(); it++) {
        int mount = find(ts, *it);
        if (mount != *it) {
            to_delete.push_back(*it);
        }
        if (!Vis[mount]) {
            tarjan(mount, t, ts, te);
        }
        if (!outOfStack[mount]) {
            lowestOrder[now] = std::min(lowestOrder[now], lowestOrder[mount]);
        }
    }

    std::vector<int>::iterator it_delete;
    for (it_delete = to_delete.begin(); it_delete != to_delete.end(); it_delete++) {
        int mount = find(ts, *it_delete);
        if (outLabel[now].find(mount) == outLabel[now].end()) {
            outLabel[now].insert(mount);
        }
        outLabel[now].erase(*it_delete);
    }

    if (inOrder[now] == lowestOrder[now]) {
        std::vector<int> CurrentSCC;
        while (Stack.top() != now) {
            outOfStack[Stack.top()] = true;
            CurrentSCC.push_back(Stack.top());
            Stack.pop();
        }
        outOfStack[Stack.top()] = true;
        CurrentSCC.push_back(Stack.top());
        Stack.pop();
        std::vector<int>::iterator it;
        for (it = CurrentSCC.begin(); it != CurrentSCC.end(); it++) {
            unioN(ts, *it, *CurrentSCC.begin(), te);
        }
        int mount = find(ts, *CurrentSCC.begin());
        for (it = CurrentSCC.begin(); it != CurrentSCC.end(); it++) {
            if (*it == mount) {
                continue;
            }
            std::unordered_set<int>::iterator it1;
            for (it1 = outLabel[*it].begin(); it1 != outLabel[*it].end(); it1++) {
                int mount_edge = find(ts, *it1);
                if (outLabel[mount].find(mount_edge) == outLabel[mount].end()) {
                    outLabel[mount].insert(mount_edge);
                }
            }
        }
    }

    outOrder[now] = ++t;

}

std::stringstream BaselineIndex::solve(int n, int ts, int te) {
    
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

BaselineIndex::BaselineIndex(TemporalGraph * Graph) {
    
    int start_time = time(NULL);
    start_time = time(NULL);

    n = Graph->numOfVertices();
    m = Graph->numOfEdges();
    tmax = Graph->tmax;
    L = new int *[tmax + 1];
    T = new int *[tmax + 1];
    size = new int[n];
    outOfStack = new bool[n];
    Vis = new bool[n];
    inOrder = new int[n];
    outOrder = new int[n];
    lowestOrder = new int[n];
    outLabel = new std::unordered_set<int>[n]();

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
            outLabel[u].clear();
            outOfStack[u] = 0;
            Vis[u] = 0;
            size[u] = 1;
        }

        std::vector<std::pair<int, int>>::iterator it;
        for (it = Graph->temporal_edge[ts].begin(); it != Graph->temporal_edge[ts].end(); it++) {
            if (outLabel[it->first].find(it->second) == outLabel[it->first].end()) {
                outLabel[it->first].insert(it->second);
            }
        }

        int t = 0;
        for (int u = 0; u < n; ++u) {
            if (!Vis[u]) {
                tarjan(u, t, ts, ts);
            }
        }

        for (int te = ts + 1; te <= tmax; ++te) {
            t = 0;
            for (int u = 0; u < n; ++u) {
                outOfStack[u] = 0;
                if (find(ts, u) != u) {
                    Vis[u] = 1;
                }
                else {
                    Vis[u] = 0;
                }
            }
            for (it = Graph->temporal_edge[te].begin(); it != Graph->temporal_edge[te].end(); it++) {
                int mountu = find(ts, it->first);
                int mountv = find(ts, it->second);

                // already in the same SCC, or mountu is an ancestor of mountv
                if (mountu == mountv || (outOrder[mountu] > outOrder[mountv] && inOrder[mountu] < inOrder[mountv])) {
                    continue;
                }

                // shift edges to the mounted vertices of SCCs
                if (outLabel[mountu].find(mountv) == outLabel[mountu].end()) {
                    outLabel[mountu].insert(mountv);
                }
            }
            for (int u = 0; u < n; ++u) {
                if (!Vis[u]) {
                    tarjan(u, t, ts, te);
                }
            }
        }
        putProcess(double(ts) / tmax, difftime(time(NULL), start_time));
    }

    delete [] size;
    delete [] outOfStack;
    delete [] Vis;
    delete [] inOrder;
    delete [] outOrder;
    delete [] lowestOrder;
    delete [] outLabel;

}

BaselineIndex::~BaselineIndex() {

    for (int ts = 0; ts <= tmax; ++ts) {
        delete [] L[ts];
        delete [] T[ts];
    }

    delete [] L;
    delete [] T;

}

void baseline(BaselineIndex * Index, int vertex_num, char * query_file, char * output_file) {

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