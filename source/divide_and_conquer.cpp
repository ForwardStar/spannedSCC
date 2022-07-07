#include "divide_and_conquer.h"
#include "commonfunctions.h"


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

void DCindex::clear(){
    for (int u = 0; u < n; ++u) {
        outLabel[u].clear();
        outOfStack[u] = 0;
        Vis[u] = false;
    }
}

void DCindex::tarjan(int now, int t, int ts, int te) {

    inOrder[now] = ++t;
    lowestOrder[now] = t;
    Vis[now] = true;
    Stack.push(now);

    std::vector<int> to_delete;

    std::unordered_set<int>::iterator it;
    for (it = outLabel[now].begin(); it != outLabel[now].end(); it++) {
        int mount = find(ts,*it);
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

}

int DCindex::efind(int u){
    return f[u]==u?f[u]:f[u]=efind(f[u]);
}

void DCindex::tarjan2(int now, int t) {

    inOrder[now] = ++t;
    lowestOrder[now] = t;
    Vis[now] = true;
    Stack.push(now);

    std::vector<int> to_delete;

    std::unordered_set<int>::iterator it;
    for (it = outLabel[now].begin(); it != outLabel[now].end(); it++) {
        int mount = efind(*it);
        if (mount != *it) {
            to_delete.push_back(*it);
        }
        if (!Vis[mount]) {
            tarjan2(mount, t);
        }
        if (!outOfStack[mount]) {
            lowestOrder[now] = std::min(lowestOrder[now], lowestOrder[mount]);
        }
    }

    std::vector<int>::iterator it_delete;
    for (it_delete = to_delete.begin(); it_delete != to_delete.end(); it_delete++) {
        int mount = efind(*it_delete);
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
            int x=efind(*it),y=efind(*CurrentSCC.begin());
            if(x!=y)f[x]=y;
        }
        int mount = efind(*CurrentSCC.begin());
        for (it = CurrentSCC.begin(); it != CurrentSCC.end(); it++) {
            if (*it == mount) {
                continue;
            }
            std::unordered_set<int>::iterator it1;
            for (it1 = outLabel[*it].begin(); it1 != outLabel[*it].end(); it1++) {
                int mount_edge = efind(*it1);
                if (outLabel[mount].find(mount_edge) == outLabel[mount].end()) {
                    outLabel[mount].insert(mount_edge);
                }
            }
        }
    }

}

void DCindex::div(int ts,int l,int r){
    //printf("%d %d %d\n",ts,l,r);
    /*if(ts==0){
        std::cerr<<l<<' '<<r<<'\n';
    }*/
    
    if(l==r){
        int t=0;
        clear();
        for(int u=0;u<n;u++)f[u]=F[u];
        std::unordered_set<long long>::iterator it;
        for (it = edge[l].begin(); it != edge[l].end(); it++) {
            long long u=(*it)>>32ll,v=(*it)&((1ll<<32)-1);
            int x=efind(u),y=efind(v);
            /*if(ts==0){
                std::cerr<<u<<' '<<v<<'\n';
                std::cerr<<x<<' '<<y<<'#'<<'\n';
            }*/
            if (outLabel[x].find(y) == outLabel[x].end()) {
                outLabel[x].insert(y);
            }
        }
        for(int u=0;u<n;u++){
            if(!Vis[u]){
                tarjan(u,t,ts,l);
            }
        }
        for(int i=0;i<n;i++)F[i]=find(ts,i);
        return ;
    }
    
    int mid=(l+r)>>1;
    clear();
    for(int u=0;u<n;u++)f[u]=F[u];
    for(int i=l;i<=mid;i++){
        std::unordered_set<long long>::iterator it;
        if(edge[i].empty()){
           // std::cerr<<ts<<' '<<i<<' '<<l<<' '<<r<<'\n';
            continue;
        }
        
        for (it = edge[i].begin(); it != edge[i].end(); it++) {
            long long u=(*it)>>32ll,v=(*it)&((1ll<<32)-1);
            int x=efind(u),y=efind(v);
            /*if(ts==0){
                std::cerr<<u<<' '<<v<<'\n';
                std::cerr<<x<<' '<<y<<'#'<<'\n';
            }*/
            if (outLabel[x].find(y) == outLabel[x].end()) {
                outLabel[x].insert(y);
            }
        }
    }
    
    
    for(int u=0;u<n;u++){
        if(!Vis[u]){
            int t=0;
            tarjan2(u,t);
        }
    }
    
    //std::cerr<<ts<<' '<<l<<' '<<r<<'\n';
    for(int i=l;i<=mid;i++){
        std::unordered_set<long long>::iterator it;
        it = edge[i].begin();
        while( it != edge[i].end()) {
            long long u=(*it)>>32,v=(*it)&((1ll<<32)-1);
            long long x=efind(u),y=efind(v);
            if(x!=y){
                /*if(ts==0){
                    std::cerr<<l<<' '<<mid<<'\n';
                    std::cerr<<u<<' '<<v<<'\n';
                    std::cerr<<x<<' '<<y<<'!'<<'\n';
                }*/
                edge[mid+1].insert((long long)(x<<32ll)+y);
                edge[i].erase(it++);
            }
            else ++it;
        }
    }
    div(ts,l,mid);
    div(ts,mid+1,r);
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
    edge = new std::unordered_set<long long>[n]();
    f = new int[n];
    F = new int[n];
    for (int ts = 0; ts <= tmax; ++ts) {
        L[ts] = new int[n];
        T[ts] = new int[n];
        for (int u = 0; u < n; ++u) {
            L[ts][u] = u;
            T[ts][u] = ts;
        }
    }
    for(int u=0;u<n;u++)size[u]=1;
    
    for(int ts=0;ts<=tmax;ts++){
        for (int t = ts; t <= tmax; ++t) {
            edge[t].clear();
            std::vector<std::pair<int, int>>::iterator it;
            for (it = Graph->temporal_edge[t].begin(); it != Graph->temporal_edge[t].end(); it++) {
                long long u = it->first;
                long long v = it->second;
                long long alfa=(long long)(u<<32ll)+v;
            //std::cerr<<u<<' '<<v<<' '<<(alfa>>32)<<' '<<(alfa&((1ll<<32)-1))<<'\n';
                edge[t].insert(alfa);
            }
        }
        for(int i=0;i<n;i++)F[i]=i;
        div(ts,ts,tmax);
    }
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