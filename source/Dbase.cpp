#include "Dbase.h"

int DifferentBaseIndex::find(int u) {
    
    if (f[u] != u) {
        return find(f[u]);
    }

    return u;

}


void DifferentBaseIndex::kosaraju1(int now) {
    //std::cerr<<now<<' '<<t<<'\n';
    Vis[now] = 1;
    for(auto g:outLabel[now]){
        int v=find((g>>10)&(33554431ll));
        //std::cerr<<now<<' '<<v<<'\n';
        if(Vis[v]==1)continue;
        kosaraju1(v);
        
    }
    Sta[++top]= now;
}


void DifferentBaseIndex::kosaraju5(int now) {
    //std::cerr<<now<<' '<<t<<'\n';
    Vis[now] = 1;
    for(auto g:outLabel[now]){
        int v=(g>>10)&(33554431ll);
        //std::cerr<<now<<'!'<<v<<'\n';
        if(Vis[v]==1)continue;
        kosaraju5(v);
        
    }
    Sta[++top]= now;
}

void DifferentBaseIndex::kosaraju3(int now) {
    CC.push_back(now);
    Vis[now] = 1;
    for(auto g:outLabel2[now]){
        long long v=g>>35;
        //std::cerr<<now<<' '<<v<<'\n';
        if(Vis[v]==1)continue;
        kosaraju3(v);
    }
}
void DifferentBaseIndex::kosaraju2(int now,int ts){
    Vis[now]=col;
    for(auto g:outLabel2[now]){
        int v=find(g>>35);
        if(Vis[v]==0){
            //std::cerr<<now<<' '<<v<<' '<<col<<'!'<<'\n';
            S[ts][ts].insert(g);
            kosaraju2(v,ts);
        }
    }
}
void DifferentBaseIndex::kosaraju4(int now, int ori, int ts){
    Vis2[now]=1;
    f[now]=ori;
    for(auto g:outLabel[now]){
        int v=find((g>>10)&(33554431ll));
        if(Vis2[v]){
            continue;
        }
        if(Vis[v]==Vis[now]){
            S[ts][ts].insert(g);
            kosaraju4(v,ori,ts);
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
    int cnt=0;
    for(int i=0;i<=ts;i++){
        for(int j=ts;j<=tmax;j++){
            if(S[i][j].empty())continue;
            for(auto g:S[i][j]){
                long long tim=g&(1023);
                if(tim>te)continue;
                long long u=(g>>35),v=(g>>10)&(33554431ll);
                outLabel[u].insert(g);
                outLabel2[v].insert(g); 
            }
        }
    }
    for(int u=0;u<n;u++){
        if(!Vis[u]){
            int t=0;
            kosaraju5(u);
        }
    }
    for(int u=0;u<n;u++)Vis[u]=0;

    while(top){
        int t=0;
        int u=Sta[top];top--;
        if(Vis[u])continue;
        CC.clear();
        kosaraju3(u);
        std::sort(CC.begin(),CC.end());
        for(auto v:CC){
            CurrentCC[CC[0]].push_back(v);
        }
    }
    for (int u = 0; u < n; ++u) {
        if (CurrentCC[u].size() == 0) {
            continue;
        }
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
    Sta = new int[n];
    Vis = new int[n];
    S = new std::unordered_set<long long> *[tmax+1]();
    outLabel = new std::unordered_set<long long>[n]();
    outLabel2 = new std::unordered_set<long long>[n]();
    Vis2 = new int[n];
    f = new int[n];
    top=0;
    for (int ts = 0; ts <= tmax; ++ts) {
        S[ts] = new std::unordered_set<long long> [tmax+1]();
        for(int te=0;te<=tmax;te++)S[ts][te].max_load_factor(10);
    }
    for (int ts = 0; ts <= tmax; ++ts) {
        for(int u=0;u<n;u++){
            outLabel[u].clear();
            outLabel2[u].clear();
            f[u]=u;
        }
        for(int t=ts;t<=tmax;t++){
            col=0;
            std::vector<std::pair<int, int>>::iterator it;
            for (it = Graph->temporal_edge[t].begin(); it != Graph->temporal_edge[t].end(); it++) {
                int u=find(it->first),v=find(it->second);
                if(u==v)continue;
                long long g=(((long long)it->first)<<35)+(((long long)it->second)<<10)+t;
                outLabel[u].insert(g);
                outLabel2[v].insert(g);
            }
            for(int u=0;u<n;u++){
                Vis2[u]=0;
                Vis[u]=0;
            }
            for(int u=0;u<n;u++){
                int g=find(u);
                if(!Vis[g]){
                    int t=0,top=0;
                    kosaraju1(g);
                }
            }
            for(int u=0;u<n;u++){
                Vis[u]=0;
            }
            while(top){
                int u=Sta[top];top--;
                //std::cerr<<f[u]<<'\n';
                int g=find(u);
                if(Vis2[g])continue;
                col++;
                int t=0;
                kosaraju2(g,ts);
                kosaraju4(g,g,ts);
                
            }
            //std::cerr<<ts<<' '<<t<<'\n';
            //for(int i=0;i<n;i++)std::cerr<<i<<' '<<f[i]<<' '<<Vis[i]<<'\n';
            for(int u=0;u<n;u++){
                int p=find(u);
                if(p==u){
                    for(auto g:outLabel2[u]){
                        long long v=g>>35;
                        if(find(u)==find(v)){
                            outLabel2[u].erase(g);
                        }
                    }
                    for(auto g:outLabel[u]){
                        long long v=(g>>10)&(33554431ll);
                        if(find(u)==find(v)){
                            outLabel[u].erase(g);
                        }
                    }
                }
                else{
                    for(auto g:outLabel2[u]){
                        long long v=g>>35;
                        if(find(u)!=find(v)){
                            outLabel2[p].insert(g);
                        }
                    }
                    for(auto g:outLabel[u]){
                        long long v=(g>>10)&(33554431ll);
                        if(find(u)!=find(v)){
                            outLabel[p].insert(g);
                        }
                    }
                    outLabel2[u].clear();
                    outLabel[u].clear();
                }
            }
        }
        for(auto g:S[ts][ts]){
            //std::cerr<<((g>>10)&(33554431ll))<<' '<<(g>>35)<<' '<<(g&1023)<<'\n';
            int flag=0;
            for(int lt=0;lt<ts;lt++){
                if(S[lt][ts-1].find(g)!=S[lt][ts-1].end()){
                    S[lt][ts].insert(g);
                    S[lt][ts-1].erase(g);
                    flag=1;
                    break;
                }
            }
            if(flag){
                S[ts][ts].erase(g);
            }
        }
        for(int lt=0;lt<ts;lt++){
            S[lt][ts-1].rehash(10);
        }
        S[ts][ts].rehash(10);
        putProcess(double(ts) / tmax, difftime(time(NULL), start_time));
    }
    /*int cnt=0;
    for(int ts=0;ts<=tmax;ts++){
        for(int te=ts;te<=tmax;te++){
            cnt+=S[ts][te].size();
            for(auto g:S[ts][te]){
                std::cerr<<((g>>10)&(33554431ll))<<' '<<(g>>35)<<' '<<(g&1023)<<' '<<ts<<' '<<te<<'\n';
            }
        }
    }
    std::cerr<<cnt<<'\n';*/
    delete [] f;
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