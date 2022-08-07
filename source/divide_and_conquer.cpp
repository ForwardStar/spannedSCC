#include "divide_and_conquer.h"
#include "commonfunctions.h"

int DCIndex::fastFind(int u) {

    return f[u]==u?f[u]:f[u]=fastFind(f[u]);

}

void DCIndex::fastUnion(int u, int v) {

    int mountu = fastFind(u);
    int mountv = fastFind(v);

    if (mountu != mountv) {
        if (sz[mountu] < sz[mountv]) {
            std::swap(mountu, mountv);
        }
        f[mountu] = mountv;
        sz[mountv]==sz[mountu]? sz[mountv]++ : 1;
    }

}

int DCIndex::find(int ts, int u) {
    
    if (L[ts][u] != u) {
        return find(ts, L[ts][u]);
    }

    return u;

}

void DCIndex::unioN(int ts, int u, int v, int t) {

    int mount_u = find(ts, u);
    int mount_v = find(ts, v);

    // Already in the same connected component.
    if (mount_u == mount_v) {
        return;
    }

    // Merge the smaller connected component into the larger one.
    if (size[mount_u] < size[mount_v]) {
        std::swap(mount_u, mount_v);
    }
    size[mount_u]==size[mount_v] ? size[mount_u]++ : 1;
    
    L[ts][mount_v] = mount_u;
    T[ts][mount_v] = t;

}

bool DCIndex::hashfind(int u,int v){
    for(auto g: outLabel[u][v%10]){
        if(g==v)return true;
    }
    return false;
}

void DCIndex::tarjan(int now, int &t, int &te) {

    inOrder[now] = ++t;
    lowestOrder[now] = t;
    Vis[now] = true;
    Stack.push(now);

    //std::vector<int> to_delete;
    std::vector<int>::iterator it;
    for(int j=0;j<10;j++)
    for (it = outLabel[now][j].begin(); it != outLabel[now][j].end(); it++) {
        int mount = fastFind(*it);
        /*if (mount != *it) {
            to_delete.push_back(*it);
        }*/
        if (!Vis[mount]) {
            tarjan(mount, t, te);
        }
        if (!outOfStack[mount]) {
            lowestOrder[now] = std::min(lowestOrder[now], lowestOrder[mount]);
        }
    }

    /*std::vector<int>::iterator it_delete;
    for (it_delete = to_delete.begin(); it_delete != to_delete.end(); it_delete++) {
        int mount = fastFind(*it_delete);
        if (!hashfind(now,mount)) {
            outLabel[now][mount%10].push_back(mount);
        }
        std::vector<int>::iterator iter;
        for(iter = outLabel[now][(*it_delete)%10].begin(); iter!=outLabel[now][(*it_delete)%10].end();iter++){
            if((*iter) == (*it_delete)){
                outLabel[now][(*it_delete)%10].erase(iter);
                break;
            }
        }
    }*/

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
            if (fastFind(*it) != fastFind(*CurrentSCC.begin())) {
                toBeMerged[te].push_back(std::pair<int, int>(*it, *CurrentSCC.begin()));
            }
            fastUnion(*it, *CurrentSCC.begin());
        }
        int mount = fastFind(*CurrentSCC.begin());
        //for (it = CurrentSCC.begin(); it != CurrentSCC.end(); it++) {
            for(int j=0;j<10;j++){
                outLabel[mount][j].clear();
            }
        //}
        CurrentSCC.clear();
        //std::vector<int> (CurrentSCC).swap(CurrentSCC);
    }

    outOrder[now] = ++t;

}



DCIndex::~DCIndex() {

    for (int ts = 0; ts <= tmax; ++ts) {
        delete [] L[ts];
        delete [] T[ts];
    }

    delete [] L;
    delete [] T;

}

DCIndex::DCIndex(TemporalGraph * Graph) {

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
    edges = new std::list<std::pair<int, int>>[tmax + 1]();
    toBeMerged = new std::vector<std::pair<int, int>>[tmax + 1]();
    outLabel = new std::vector<int> *[n]();
    for(int u=0;u<n;u++){
        outLabel[u] = new std::vector<int>[10]();
    }
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
            edges[t].clear();
            std::vector<std::pair<int, int>>::iterator it;
            for (it = Graph->temporal_edge[t].begin(); it != Graph->temporal_edge[t].end(); it++) {
                edges[t].push_back(std::pair<int, int>(it->first, it->second));
            }
        }

        for (int i = tmax - ts + 1; i >= 1; i >>= 1) {
            for (int u = 0; u < n; ++u) {
                f[u] = u;
                sz[u] = 1;
                for(int j=0;j<10;j++)
                outLabel[u][j].clear();
            }

            // start BFS
            for (int j = ts; j <= tmax; j += i) {
                int te = std::min(tmax, j + i - 1);

                // absorb edges in neighbour nodes
                if (!hasTarjaned[te]) {
                    for (int t = j; t <= std::min(tmax, j + i - 1); ++t) {
                        std::list<std::pair<int, int>>::iterator it;
                        for (it = edges[t].begin(); it != edges[t].end();) {
                            int mountu = fastFind(it->first);
                            int mountv = fastFind(it->second);
                            if (!hashfind(mountu,mountv)) {
                                outLabel[mountu][mountv%10].push_back(mountv);
                                it++;
                            }
                            else {
                                edges[t].erase(it++);
                            }
                        }
                    }
                }

                // perform SCC tarjan
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
                            t=0;
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
                    std::list<std::pair<int, int>>::iterator it;
                    for (it = edges[t].begin(); it != edges[t].end();) {
                        int mountu = fastFind(it->first);
                        int mountv = fastFind(it->second);
                        if (mountu == mountv) {
                            it++;
                        }
                        else {
                            if (j + i <= tmax) {
                                edges[j + i].push_back(std::pair<int, int>(mountu, mountv));
                            }
                            edges[t].erase(it++);
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
    delete [] edges;
    delete [] toBeMerged;
    delete [] outLabel;
    delete [] f;
    delete [] sz;
    
}

std::stringstream DCIndex::solve(int n, int ts, int te) {

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



void DC(DCIndex * Index, int vertex_num, char * query_file, char * output_file) {

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