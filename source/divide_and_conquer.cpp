#include "divide_and_conquer.h"
#include "commonfunctions.h"

int DCindex::fastFind(int u) {

    if (f[u] == u) {
        return u;
    }
    int mountu = fastFind(f[u]);
    f[u] = mountu;
    sz[mountu] = sz[u] + 1;
    return mountu;

}

void DCindex::fastUnion(int u, int v) {

    int mountu = fastFind(u);
    int mountv = fastFind(v);

    if (mountu != mountv) {
        if (sz[mountu] < sz[mountv]) {
            std::swap(mountu, mountv);
        }
        f[mountu] = mountv;
        sz[mountv] = sz[mountu] + 1;
    }

}

int DCindex::find(int ts, int u) {
    
    if (L[ts][u] != u) {
        return find(ts, L[ts][u]);
    }

    return u;

}

void DCindex::unioN(int ts, int u, int v, int t) {

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

void DCindex::tarjan(int now, int &t, int &te) {

    inOrder[now] = ++t;
    lowestOrder[now] = t;
    Vis[now] = true;
    Stack.push(now);

    std::vector<int> to_delete;
    std::unordered_set<int>::iterator it;
    for (it = outLabel[now].begin(); it != outLabel[now].end(); it++) {
        int mount = fastFind(*it);
        if (mount != *it) {
            to_delete.push_back(*it);
        }
        if (!Vis[mount]) {
            tarjan(mount, t, te);
        }
        if (!outOfStack[mount]) {
            lowestOrder[now] = std::min(lowestOrder[now], lowestOrder[mount]);
        }
    }

    std::vector<int>::iterator it_delete;
    for (it_delete = to_delete.begin(); it_delete != to_delete.end(); it_delete++) {
        int mount = fastFind(*it_delete);
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
            fastUnion(*it, *CurrentSCC.begin());
            toBeMerged[te].push_back(std::pair<int, int>(*it, *CurrentSCC.begin()));
        }
        int mount = fastFind(*CurrentSCC.begin());
        for (it = CurrentSCC.begin(); it != CurrentSCC.end(); it++) {
            if (*it == mount) {
                continue;
            }
            std::unordered_set<int>::iterator it1;
            for (it1 = outLabel[*it].begin(); it1 != outLabel[*it].end(); it1++) {
                int mount_edge = fastFind(*it1);
                if (outLabel[mount].find(mount_edge) == outLabel[mount].end()) {
                    outLabel[mount].insert(mount_edge);
                }
            }
        }
        CurrentSCC.clear();
        std::vector<int> (CurrentSCC).swap(CurrentSCC);
    }

    outOrder[now] = ++t;

}

DCindex::~DCindex() {

    for (int ts = 0; ts <= tmax; ++ts) {
        delete [] L[ts];
        delete [] T[ts];
    }

    delete [] L;
    delete [] T;

}

DCindex::DCindex(TemporalGraph * Graph) {

    int start_time = time(NULL);

    n = Graph->numOfVertices();
    m = Graph->numOfEdges();
    tmax = Graph->tmax;
    L = new int *[tmax + 1];
    T = new int *[tmax + 1];
    size = new int[n];
    outOfStack = new bool[n];
    Vis = new bool[n];
    hasTarjaned = new bool[tmax + 1];
    inOrder = new int[n];
    outOrder = new int[n];
    lowestOrder = new int[n];
    hashedEdges = new std::unordered_set<long long>[tmax + 1]();
    toBeMerged = new std::vector<std::pair<int, int>>[tmax + 1]();
    outLabel = new std::unordered_set<int>[n]();
    f = new int[n];
    sz = new int[n];
    for (int ts = 0; ts <= tmax; ++ts) {
        L[ts] = new int[n];
        T[ts] = new int[n];
        for (int u = 0; u < n; ++u) {
            L[ts][u] = u;
            T[ts][u] = ts;
        }
    }

    for (int ts = 0; ts <= tmax; ts++) {
        for (int u = 0; u < n; ++u) {
            size[u] = 1;
        }

        for (int t = ts; t <= tmax; ++t) {
            hasTarjaned[t] = false;
            toBeMerged[t].clear();
            hashedEdges[t].clear();
            std::vector<std::pair<int, int>>::iterator it;
            for (it = Graph->temporal_edge[t].begin(); it != Graph->temporal_edge[t].end(); it++) {
                hashedEdges[t].insert(it->first * n + it->second);
            }
        }

        for (int i = tmax - ts + 1; i >= 1; i >>= 1) {
            for (int u = 0; u < n; ++u) {
                f[u] = u;
                sz[u] = 1;
                outLabel[u].clear();
            }

            // start BFS
            for (int j = ts; j <= tmax; j += i) {
                // absorb edges in neighbour nodes
                for (int t = j; t <= std::min(tmax, j + i - 1); ++t) {
                    std::unordered_set<long long>::iterator it;
                    for (it = hashedEdges[t].begin(); it != hashedEdges[t].end(); it++) {
                        int u = *it / n;
                        int v = *it % n;
                        int mountu = fastFind(u);
                        int mountv = fastFind(v);
                        if (j != ts) {
                            if (mountu == mountv || inOrder[mountu] < inOrder[mountv] && outOrder[mountu] > outOrder[mountv]) {
                                continue;
                            }
                        }
                        outLabel[mountu].insert(mountv);
                    }
                }

                // perform SCC tarjan
                int te = std::min(tmax, j + i - 1);
                if (!hasTarjaned[te]) {
                    for (int u = 0; u < n; u++) {
                        if (fastFind(u) != u) {
                            Vis[u] = 1;
                        }
                        else {
                            Vis[u] = 0;
                        }
                        outOfStack[u] = 0;
                    }
                    hasTarjaned[te] = true;
                    int t = 0;
                    for (int u = 0; u < n; u++) {
                        if (!Vis[u]) {
                            tarjan(u, t, te);
                        }
                    }
                }
                else {
                    std::vector<std::pair<int, int>>::iterator it;
                    for (it = toBeMerged[te].begin(); it != toBeMerged[te].end(); it++) {
                        fastUnion(it->first, it->second);
                    }
                }

                if (i == 1) {
                    std::vector<std::pair<int, int>>::iterator it;
                    for (it = toBeMerged[te].begin(); it != toBeMerged[te].end(); it++) {
                        unioN(ts, it->first, it->second, te);
                    }
                }

                // select an edge set for next level
                for (int t = j; t <= std::min(tmax, j + i - 1); ++t) {
                    std::unordered_set<long long>::iterator it;
                    for (it = hashedEdges[t].begin(); it != hashedEdges[t].end();) {
                        int u = *it / n;
                        int v = *it % n;
                        int mountu = fastFind(u);
                        int mountv = fastFind(v);
                        if (mountu == mountv) {
                            it++;
                        }
                        else {
                            if (j + i <= tmax) {
                                hashedEdges[j + i].insert((long long)mountu * n + mountv);
                            }
                            hashedEdges[t].erase(it++);
                        }
                    }
                }
            }
        }
        putProcess(double(ts) / tmax, difftime(time(NULL), start_time));
    }

    delete [] size;
    delete [] outOfStack;
    delete [] Vis;
    delete [] hasTarjaned;
    delete [] inOrder;
    delete [] outOrder;
    delete [] lowestOrder;
    delete [] hashedEdges;
    delete [] toBeMerged;
    delete [] outLabel;
    delete [] f;
    delete [] sz;
    
}

std::stringstream DCindex::solve(int n, int ts, int te) {

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



void DC(DCindex * Index, int vertex_num, char * query_file, char * output_file) {

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