#include "optimized.h"

int OptimizedIndex::find(int u) {
    
    if (f[u] != u) {
        return find(f[u]);
    }

    return u;

}


void OptimizedIndex::kosaraju1(int now) {
    //std::cerr<<now<<' '<<t<<'\n';
    Vis[now] = 1;
    for(auto g:outLabel[now]){
        int v=find((g>>12)&(33554431ll));
        //std::cerr<<now<<' '<<v<<'\n';
        if(Vis[v]==1)continue;
        kosaraju1(v);
        
    }
    Sta[++top]= now;
}


void OptimizedIndex::kosaraju5(int now) {
    //std::cerr<<now<<' '<<t<<'\n';
    Vis[now] = 1;
    for(auto g:outLabel[now]){
        int v=(g>>12)&(33554431ll);
        //std::cerr<<now<<'!'<<v<<'\n';
        if(Vis[v]==1)continue;
        kosaraju5(v);
        
    }
    Sta[++top]= now;
}

void OptimizedIndex::kosaraju3(int now) {
    CC.push_back(now);
    Vis[now] = 1;
    for(auto g:outLabel2[now]){
        long long v=g>>37;
        //std::cerr<<now<<' '<<v<<'\n';
        if(Vis[v]==1)continue;
        kosaraju3(v);
    }
}
void OptimizedIndex::kosaraju2(int now,int ts){
    Vis[now]=col;
    for(auto g:outLabel2[now]){
        int v=find(g>>37);
        if(Vis[v]==0){
            //std::cerr<<now<<' '<<v<<' '<<col<<'!'<<'\n';
            S[ts][ts].insert(g);
            kosaraju2(v,ts);
        }
    }
}
void OptimizedIndex::kosaraju4(int now, int ori, int ts){
    
    Vis2[now]=1;
    f[now]=ori;
    CC.push_back(now);
    for(auto g:outLabel[now]){
        int v=find((g>>12)&(33554431ll));
        if(Vis2[v]){
            continue;
        }
        if(Vis[v]==Vis[now]){
            S[ts][ts].insert(g);
            kosaraju4(v,ori,ts);
        }
    }
}
std::stringstream OptimizedIndex::solve(int n, int ts, int te) {
    
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
            if(G[i][j].empty())continue;
            for(auto g:G[i][j]){
                long long tim=g&(4095);
                if(tim>te)continue;
                cnt++;
                long long u=(g>>37),v=(g>>12)&(33554431ll);
                outLabel[u].push_back(g);
                outLabel2[v].push_back(g); 
            }
        }
    }
    //std::cerr<<ts<<' '<<te<<' '<<cnt<<'\n';
    for(int u=0;u<n;u++){
        if(!Vis[u]){
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
        CurrentCC[CC[0]]=CC;
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

OptimizedIndex::OptimizedIndex(TemporalGraph * Graph) {
    
    int start_time = time(NULL);
    start_time = time(NULL);

    n = Graph->numOfVertices();
    m = Graph->numOfEdges();
    tmax = Graph->tmax;
    Sta = new int[n];
    Vis = new int[n];
    S = new std::set<long long> *[tmax+1]();
    edge = new std::vector<long long> [tmax+1]();
    G = new std::vector<long long> *[tmax+1]();
    outLabel = new std::vector<long long>[n]();
    outLabel2 = new std::vector<long long>[n]();
    Vis2 = new int[n];
    f = new int[n];
    top=0;
    for(int t=0;t<=tmax;t++){
        std::vector<std::pair<int, int>>::iterator iter;
            for (iter = Graph->temporal_edge[t].begin(); iter != Graph->temporal_edge[t].end(); iter++) {
                int u=iter->first,v=iter->second;
                long long g=(((long long)iter->first)<<37)+(((long long)iter->second)<<12)+t;
                edge[t].push_back(g);
            }
            Graph->temporal_edge[t].clear();
            std::vector<std::pair<int,int>>().swap(Graph->temporal_edge[t]);
    }
    for (int ts = 0; ts <= tmax; ++ts) {
        S[ts] = new std::set<long long> [tmax+1]();
        G[ts] = new std::vector<long long> [tmax+1]();
        //std::cerr<<ts<<'\n';
        for(int u=0;u<n;u++){
            outLabel[u].clear();
            outLabel2[u].clear();
            f[u]=u;
        }
        for(int t=ts;t<=tmax;t++){
            std::vector<long long>::iterator it;
            tmpedge.clear();
            for (it = edge[t].begin(); it != edge[t].end(); it++) {
                long long g=*it;
                int u=find(g>>37),v=find((g>>12)&(33554431ll)),tim=g&(4095);
                if(u==v){tmpedge.push_back(g);continue;}
                if(tim<ts)continue;
                outLabel[u].push_back(g);
                outLabel2[v].push_back(g);
            }
            edge[t]=tmpedge;
            for(int u=0;u<n;u++){
                Vis2[u]=0;
                Vis[u]=0;
            }
            for(int u=0;u<n;u++){
                int g=find(u);
                if(!Vis[g]){
                    int top=0;
                    kosaraju1(g);
                }
            }
            for(int u=0;u<n;u++){
                Vis[u]=0;
            }
            col=0;
            //std::cerr<<ts<<' '<<t<<'\n';
            while(top){
                int u=Sta[top];top--;
                //std::cerr<<u<<' '<<find(u)<<'\n';
                int g=find(u);
                if(Vis2[g])continue;
                col++;
                CC.clear();
                kosaraju2(g,ts);
                kosaraju4(g,g,ts);
                std::vector<long long> tmp;
                tmp.clear();
                for(auto u:CC){
                    std::vector<long long>::iterator iter;
                    for(iter=outLabel2[u].begin();iter!=outLabel2[u].end();iter++){
                        long long v=(*iter)>>37;
                        if(find(v)!=g){
                            tmp.push_back(*iter);
                        }
                        else{
                            //std::cerr<<u<<' '<<v<<' '<<((*iter)&4095)<<' '<<t<<'\n';
                            edge[t].push_back(*iter);
                        }
                    }
                }
                for(auto u:CC){
                    outLabel2[u].clear();
                    std::vector<long long>().swap(outLabel2[u]);
                }
                outLabel2[g]=tmp;
                tmp.clear();
                for(auto u:CC){
                    std::vector<long long>::iterator iter;
                    for(iter=outLabel[u].begin();iter!=outLabel[u].end();iter++){
                        long long v=((*iter)>>12)&(33554431ll);
                        if(find(v)!=g){
                            tmp.push_back(*iter);
                        }
                    }
                }
                for(auto u:CC){
                    outLabel[u].clear();
                    std::vector<long long>().swap(outLabel[u]);
                }
                outLabel[g]=tmp;
                tmp.clear();
                //std::vector<long long>().swap(tmp);
            }
            
            /*std::cerr<<ts<<' '<<t<<'\n';
            for(int i=0;i<n;i++)std::cerr<<i<<' '<<find(i)<<'\n';
            tmpedge.clear();
            for (it = edge[t].begin(); it != edge[t].end();) {
                long long g=*it;
                long long u=find(g>>37),v=find((g>>12)&(33554431ll)),tim=g&(4095);
                if(tim <= ts){
                    //edge[t].erase(it++);
                    it++;
                    continue;
                }
                if(u==v){tmpedge.push_back(g);it++;continue;}
                if(t<tmax && tim>ts){
                    it++;
                    continue;
                }
                tmpedge.push_back(g);
                it++;
            }
            edge[t]=tmpedge;
            tmpedge.clear();*/
        }
        //std::cerr<<S[ts][ts].size()<<'\n';
        std::set<long long>::iterator iter;
        for(iter=S[ts][ts].begin();iter!=S[ts][ts].end();){
            int flag=0;
            long long g=*iter;
            //std::cerr<<((g>>12)&(33554431ll))<<' '<<(g>>37)<<' '<<(g&4095)<<'\n';
            for(int lt=0;lt<ts;lt++){
                if(S[lt][ts-1].find(g)!=S[lt][ts-1].end()){
                    S[lt][ts].insert(g);
                    S[lt][ts-1].erase(g);
                    flag=1;
                    break;
                }
            }
            if(flag){
                if(S[ts][ts].size()==1){
                    S[ts][ts].clear();
                    break;
                }
                S[ts][ts].erase(iter++);
            }
            else iter++;
        }
        for(int lt=0;lt<ts;lt++){
            //std::cerr<<lt<<' '<<ts<<' '<<S[lt][ts-1].size()<<'\n';
            std::set<long long>::iterator iter;
            for(iter=S[lt][ts-1].begin();iter!=S[lt][ts-1].end();iter++){
                G[lt][ts-1].push_back(*iter);
            }
            S[lt][ts-1].clear();
        }
        putProcess(double(ts) / tmax, difftime(time(NULL), start_time));
    }
    for(int lt=0;lt<=tmax;lt++){
        for(auto g:S[lt][tmax]){
            G[lt][tmax].push_back(g);
        }
        S[lt][tmax].clear();
    }
    int cnt=0,tot=0;
    for(int ts=0;ts<=tmax;ts++){
        for(int te=ts;te<=tmax;te++){
            cnt+=G[ts][te].size();
            //std::cerr<<ts<<' '<<te<<' '<<G[ts][te].size()<<'\n';
        }
    }
    std::cout<<"number of effective edges: "<<cnt<<std::endl;
    std::cout<<"space acquired: "<<cnt*8<<" bytes"<<std::endl;
    delete [] f;
    delete [] edge;

}

OptimizedIndex::~OptimizedIndex() {

    delete [] L;
    delete [] T;

}

void optimized(OptimizedIndex * Index, int vertex_num, char * query_file, char * output_file) {

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