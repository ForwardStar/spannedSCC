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

void DCindex::tarjanOnLeafNode(int now, int &t, int ts, int te) {

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
            tarjanOnLeafNode(mount, t, ts, te);
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
            unioN(ts, *it, *CurrentSCC.begin(), te);
            fastUnion(*it, *CurrentSCC.begin());
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

}

void DCindex::tarjanOnNonLeafNode(int now, int &t) {

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
            tarjanOnNonLeafNode(mount, t);
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

}

DCindex::~DCindex() {

    for (int ts = 0; ts <= tmax; ++ts) {
        delete [] L[ts];
        delete [] T[ts];
    }

    delete [] L;
    delete [] T;

}

DCindex::DCindex(TemporalGraph * Graph){
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
    lowestOrder = new int[n];
    outLabel = new std::unordered_set<int>[n]();
    edge = new std::unordered_set<long long>[tmax + 1]();
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
        for (int t = ts; t <= tmax; t++) {
            edge[t].clear();
            std::vector<std::pair<int, int>>::iterator it;
            for (it = Graph->temporal_edge[t].begin(); it != Graph->temporal_edge[t].end(); it++) {
                long long u = it->first;
                long long v = it->second;
                long long alfa = u * (n + 1) + v;
                if (edge[t].find(alfa) == edge[t].end()) {
                    edge[t].insert(alfa);
                }
            }
        }

        for (int u = 0; u < n; ++u) {
            size[u] = 1;
        }

        for (int i = (tmax - ts) >> 1; i >= 1; i >>= 1) {
            for (int u = 0; u < n; ++u) {
                f[u] = u;
                sz[u] = 1;
                outLabel[u].clear();
            }
            for (int j = ts; j <= tmax; j += i) {   
                if (i > 1 && j + i - 1 > tmax) {
                    continue;
                }
                for (int u = 0; u < n; u++) {
                    if (fastFind(u) != u) {
                        Vis[u] = 1;
                    }
                    else {
                        Vis[u] = 0;
                    }
                    outOfStack[u] = 0;
                }
                for (int t = j; t <= std::min(tmax, j + i - 1); ++t) {
                    std::unordered_set<long long>::iterator it;
                    for (it = edge[t].begin(); it != edge[t].end(); it++) {
                        long long u = (*it) / (n + 1);
                        long long v = (*it) - u * (n + 1);
                        int x = fastFind(u), y = fastFind(v);
                        if (outLabel[x].find(y) == outLabel[x].end()) {
                            outLabel[x].insert(y);
                        }
                    }
                }
                for (int u = 0; u < n; u++) {
                    int t = 0;
                    if (i > 1) {
                        if (!Vis[u]) {
                            tarjanOnNonLeafNode(u, t);
                        }
                    }
                    else {
                        if (!Vis[u]) {
                            tarjanOnLeafNode(u, t, ts, j);
                        }
                    }
                }
                for (int t = j; t <= std::min(tmax, j + i - 1); t++) {
                    std::unordered_set<long long>::iterator it;
                    for (it = edge[t].begin(); it != edge[t].end();) {
                        long long u = (*it) / (n + 1);
                        long long v = (*it) - u * (n + 1);
                        int x = fastFind(u), y = fastFind(v);
                        if (x == y) {
                            it++;
                        }
                        else {
                            if (j + i <= tmax) {
                                long long alfa = u * (n + 1) + v;
                                if (edge[j + i].find(alfa) == edge[j + i].end()) {
                                    edge[j + i].insert(alfa);
                                }
                            }
                            edge[t].erase(it++);
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
    delete [] inOrder;
    delete [] lowestOrder;
    delete [] outLabel;
    delete [] edge;
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