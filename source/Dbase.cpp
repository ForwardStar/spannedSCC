#include "Dbase.h"

int DifferentBaseIndex::find(int u) {
    
    if (L[u] != u) {
        return find(L[u]);
    }

    return u;

}

void DifferentBaseIndex::unioN(int ts, int u, int v, int t) {

    int mount_u = find(u);
    int mount_v = find(v);

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
    
    L[mount_v] = mount_u;
    T[mount_v] = t;

}

void DifferentBaseIndex::kosaraju1(int now, int &t) {
    //std::cerr<<now<<' '<<t<<'\n';
    inOrder[now] = ++t;
    Vis[now] = 1;
    for(auto g:outLabel[now]){
        int v=(g>>10)&(33554431ll);
        //std::cerr<<now<<' '<<v<<'\n';
        if(Vis[v]==1)continue;
        kosaraju1(v,t);
        
    }
    Sta[++top]= now;
}


void DifferentBaseIndex::kosaraju3(int now, int &t) {
    CC.push_back(now);
    inOrder[now] = ++t;
    Vis[now] = 1;
    for(auto g:outLabel2[now]){
        long long v=g>>35;
        //std::cerr<<now<<' '<<v<<'\n';
        if(Vis[v]==1)continue;
        kosaraju3(v,t);
    }
}

void DifferentBaseIndex::kosaraju2(int now,int &t,int ts){
    Vis[now]=col;
    for(auto g:outLabel2[now]){
        int v=(g>>35);
        int tim=g&((1<<10)-1);
        if(Vis[v]==0){
            S[ts][ts].insert(g);
            kosaraju2(v,t,ts);
        }
    }
}
void DifferentBaseIndex::kosaraju4(int now,int ts){
    Vis2[now]=1;
    for(auto g:outLabel[now]){
        int v=(g>>10)&(33554431ll);
        if(Vis2[v])continue;
        if(Vis[v]==Vis[now]){
            S[ts][ts].insert(g);
            kosaraju4(v,ts);
        }
    }
}
std::stringstream DifferentBaseIndex::solve(int n, int ts, int te) {
    
    std::stringstream Ans;
    std::vector<int> *CurrentCC = new std::vector<int>[n]();
    
    Ans << "The spanned strongly connected components in [" << ts << ", " << te << "] are:\n";
    top=0;
    
    for(int u=0;u<n;u++){
        Vis[u]=0;
        outLabel[u].clear();
        outLabel2[u].clear();
    }
    
    for(int i=0;i<=ts;i++){
        for(int j=ts;j<=tmax;j++){
            if(S[i][j].empty())continue;
            for(auto g:S[i][j]){
                long long tim=g&(1023);
                if(tim>te)continue;
                long long u=(g>>35),v=(g>>10)&(33554431ll);
                //std::cerr<<u<<' '<<v<<'\n';
                outLabel[u].insert(g);
                outLabel2[v].insert(g);
            }
        }
    }
    
    for(int u=0;u<n;u++){
        if(!Vis[u]){
            int t=0;
            kosaraju1(u,t);
        }
    }
    for(int u=0;u<n;u++)Vis[u]=0;

    while(top){
        int t=0;
        int u=Sta[top];top--;
        if(Vis[u])continue;
        CC.clear();
        kosaraju3(u,t);
        std::sort(CC.begin(),CC.end());
        for(auto v:CC){
            CurrentCC[CC[0]].push_back(v);
        }
    }
    for (int u = 0; u < n; ++u) {
        if (CurrentCC[u].size() == 0) {
            continue;
        }
        //std::cerr<<u<<'\n';
        std::vector<int>::iterator it;
        Ans << "{ ";
        for (it = CurrentCC[u].begin(); it != CurrentCC[u].end(); it++) {
            Ans << *it << " ";
        }
        Ans << "}\n";
    }

    delete [] CurrentCC;
    return Ans;

}

DifferentBaseIndex::DifferentBaseIndex(TemporalGraph * Graph) {
    
    int start_time = time(NULL);
    start_time = time(NULL);

    n = Graph->numOfVertices();
    m = Graph->numOfEdges();
    tmax = Graph->tmax;
    L = new int [tmax + 1];
    T = new int [tmax + 1];
    size = new int[n];
    Sta = new int[n];
    Vis = new int[n];
    inOrder = new int[n];
    outOrder = new int[n];
    lowestOrder = new int[n];
    S = new std::unordered_set<long long> *[tmax+1]();
    outLabel = new std::unordered_set<long long>[n]();
    outLabel2 = new std::unordered_set<long long>[n]();
    Vis2 = new int[n];
    top=0;
    for (int ts = 0; ts <= tmax; ++ts) {
        S[ts] = new std::unordered_set<long long> [tmax+1]();
    }
    for (int ts = 0; ts <= tmax; ++ts) {
        for(int u=0;u<n;u++){
            outLabel[u].clear();
            outLabel2[u].clear();
        }
        for(int t=ts;t<=tmax;t++){
            for(int u=0;u<n;u++){
                Vis[u]=0;
            }
            std::vector<std::pair<int, int>>::iterator it;
            for (it = Graph->temporal_edge[t].begin(); it != Graph->temporal_edge[t].end(); it++) {
                long long g=(((long long)it->first)<<35)+(((long long)it->second)<<10)+t;
                outLabel[it->first].insert(g);
                outLabel2[it->second].insert(g);
            }
            for(int u=0;u<n;u++){
            if(!Vis[u]){
                int t=0,top=0;
                kosaraju1(u,t);
            }
        }
        for(int u=0;u<n;u++){
            Vis2[u]=0;
            Vis[u]=0;
        }
        while(top){
            int u=Sta[top];top--;
            if(Vis2[u])continue;
            col++;
            int t=0;
            kosaraju2(u,t,ts);
            kosaraju4(u,ts);
        }
        for(auto g:S[ts][ts])
        for(int lt=0;lt<ts;lt++){
            if(S[lt][ts-1].find(g)!=S[lt][ts-1].end()){
                S[lt][ts].insert(g);
                S[lt][ts-1].erase(g);
                break;
            }
        }
        }
        
        
        putProcess(double(ts) / tmax, difftime(time(NULL), start_time));
    }
    delete [] size;
    delete [] outOrder;
    delete [] lowestOrder;

}

DifferentBaseIndex::~DifferentBaseIndex() {

    delete [] L;
    delete [] T;

}

void Dbase(DifferentBaseIndex * Index, int vertex_num, char * query_file, char * output_file) {

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