#include "optimized.h"
#include<math.h>

int OptimizedIndex::find(int u) {
    
    if (f[u] != u) {
        return f[u]=find(f[u]);
    }

    return u;

}


void OptimizedIndex::kosaraju1(int now) {
    //std::cerr<<now<<' '<<t<<'\n';
    Vis[now] = 1;
    markedVertices.push_back(now);
    if(!outLabel[now].empty())
    for(auto g:outLabel[now]){
        int v=find((g.first>>12)&(33554431ll));
        //std::cerr<<now<<' '<<v<<'\n';
        if(Vis[v]==1)continue;
        kosaraju1(v);
        
    }
    Sta[++top]= now;
}


void OptimizedIndex::kosaraju5(int now) {
    //std::cerr<<now<<' '<<t<<'\n';
    Vis[now] = 1;
    markedVertices.push_back(now);
    if(!outLabel[now].empty())
    for(auto g:outLabel[now]){
        int v=(g.first>>12)&(33554431ll);
        //std::cerr<<now<<'!'<<v<<'\n';
        if(Vis[v]==1)continue;
        kosaraju5(v);
        
    }
    Sta[++top]= now;
}

void OptimizedIndex::kosaraju3(int now) {
    CC.push_back(now);
    Vis[now] = 1;
    markedVertices.push_back(now);
    if(!outLabel2[now].empty())
    for(auto g:outLabel2[now]){
        long long v=g.first>>37;
        //std::cerr<<now<<' '<<v<<'\n';
        if(Vis[v]==1)continue;
        kosaraju3(v);
    }
}
void OptimizedIndex::kosaraju2(int now,int ts){
    Vis[now]=col;
    markedVertices.push_back(now);
    if(!outLabel2[now].empty())
    for(auto g:outLabel2[now]){
        int v=find(int(g.first>>37));
        if(Vis[v]==0){
            //std::cerr<<(g.first>>37)<<' '<<((g.first>>12)&(33554431ll))<<' '<<col<<'!'<<'\n';
            //S[ts][ts].insert(g);
            key.insert(g);
            kosaraju2(v,ts);
        }
    }
}
void OptimizedIndex::kosaraju4(int now, int ori, int ts){
    
    Vis2[now]=1;
    markedVertices2.push_back(now);
    f[now]=ori;
    CC.push_back(now);
    if(!outLabel[now].empty())
    for(auto g:outLabel[now]){
        int v=find((g.first>>12)&(33554431ll));
        if(Vis2[v]){
            continue;
        }
        if(Vis[v]==Vis[now]){
            //std::cerr<<(g.first>>37)<<' '<<((g.first>>12)&(33554431ll))<<'\n';
            //S[ts][ts].insert(g);
            key.insert(g);
            kosaraju4(v,ori,ts);
        }
    }
}

int OptimizedIndex::find_an_index(int t, int ts, int te) {

    int l = 0;
    int r = actual_time[t].size() - 1;

    if (r == -1 || actual_time[t][r] < ts || actual_time[t][0] > te) {
        return -1;
    }
    
    while (l < r) {
        int mid = l + r >> 1;
        if (actual_time[t][mid] >= ts && actual_time[t][mid] <= te) {
            return mid;
        }
        else {
            if (actual_time[t][mid] < ts) {
                l = mid + 1;
            }
            else {
                r = mid;
            }
        }
    }

    if (actual_time[t][l] >= ts && actual_time[t][l] <= te) {
        return l;
    }
    else {
        return -1;
    }

}

std::stringstream OptimizedIndex::solve(int n, int ts, int te) {
    
    std::stringstream Ans;
    std::vector<int> *CurrentCC = new std::vector<int>[n]();
    markedVertices.clear();
    markedVertices2.clear();
    
    Ans << "The spanned strongly connected components in [" << ts << ", " << te << "] are:\n";

    top = 0;
    
    for (int u = 0; u < n; u++) {
        Vis[u] = 0;
        outLabel[u].clear();
        outLabel2[u].clear();
    }

    int cnt = 0;
    int l=ts/len+1;
    int r=te/len;
    for(int i=ts;i<l*len;i++){
        for(int now=0;now<G[i].size();now++){
            if(G[i][now].ts<=ts){
                std::pair<long long,int> g=G[i][now].edge;
                long long u = (g.first >> 37), v = (g.first >> 12) & (33554431ll);
                if(g.second>te) continue;
                outLabel[u].push_back(g);
                outLabel2[v].push_back(g);
            }
            else break;
        }
    }
    for(int i=l;i<r;i++){
        for(int now=0;now<Chunk[i].size();now++){
            if(Chunk[i][now].ts<=ts){
                std::pair<long long,int> g=Chunk[i][now].edge;
                long long u = (g.first >> 37), v = (g.first >> 12) & (33554431ll);
                if(g.second>te) continue;
                outLabel[u].push_back(g);
                outLabel2[v].push_back(g);
            }
            else break;
        }
    }
    for(int i=r*len;i<=te;i++){
        for(int now=0;now<G[i].size();now++){
            if(G[i][now].ts<=ts){
                std::pair<long long,int> g=G[i][now].edge;
                long long u = (g.first >> 37), v = (g.first >> 12) & (33554431ll);
                if(g.second>te) continue;
                outLabel[u].push_back(g);
                outLabel2[v].push_back(g);
            }
            else break;
        }
    }
    // for(int i=ts;i<=te;i++){
    //     for(int now=0;now<G[i].size();now++){
    //         if(G[i][now].ts<=ts){
    //             std::pair<long long,int> g=G[i][now].edge;
    //             long long u = (g.first >> 37), v = (g.first >> 12) & (33554431ll);
    //             if(g.second>te) continue;
    //             outLabel[u].push_back(g);
    //             outLabel2[v].push_back(g);
    //         }
    //         else break;
    //     }
    // }
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

bool OptimizedIndex::cmp(RES a, RES b){
    return a.ts<b.ts;
}

OptimizedIndex::OptimizedIndex(TemporalGraph * Graph, double t_fraction) {
    
    unsigned long long start_time = currentTime();

    n = Graph->numOfVertices();
    m = Graph->numOfEdges();
    tmax = Graph->tmax;
    len=sqrt(tmax);
    Sta = new int[n];
    Vis = new int[n];
    Vis2 = new int[n];
    f = new int[n];
    edge = new std::vector<std::pair<long long,int>> [tmax+1]();
    newedge = new std::vector<std::pair<long long,int>> [tmax+1]();
    G = new std::vector<RES> [tmax+1]();
    Chunk = new std::vector<RES> [tmax+1]();
    actual_time= new std::vector<int> [tmax+1]();
    outLabel = new std::vector<std::pair<long long,int>>[n]();
    outLabel2 = new std::vector<std::pair<long long,int>>[n]();
    top=0;
    S.clear();
    t1=tmax*t_fraction;
    for(int t=0;t<=t1;t++){
        std::vector<std::pair<int, int>>::iterator iter;
            for (iter = Graph->temporal_edge[t].begin(); iter != Graph->temporal_edge[t].end(); iter++) {
                int u=iter->first,v=iter->second;
                std::pair<long long,int> g=std::pair<long long,int>((((long long)iter->first)<<37)+(((long long)iter->second)<<12),t);
                edge[t].push_back(g);
            }
    }
    std::set<std::pair<long long,int>> table;
    for (int ts = 0; ts <= t1; ++ts) {
        //std::cerr<<ts<<'\n';
        for(int u=0;u<n;u++){
            outLabel[u].clear();
            outLabel2[u].clear();
            f[u]=u;
            Vis[u]=0;
            Vis2[u]=0;
        }
        for(int i=0;i<=t1;i++)newedge[i].clear();
        for(auto g:key){
            newedge[g.second].push_back(g);
        }
        table=key;
        key.clear();
        for(int t=ts;t<=t1;t++){
            //  std::cerr<<ts<<' '<<t<<'\n';
            std::vector<int> point;
            point.clear();
            std::vector<std::pair<long long,int>>::iterator it;
            tmpedge.clear();
            if(!newedge[t].empty())
            for(it = newedge[t].begin();it!=newedge[t].end();it++){
                std::pair<long long,int> g=*it;
                int u=find(g.first>>37),v=find((g.first>>12)&(33554431ll)),tim=g.second;
                if(u==v){tmpedge.push_back(g);continue;}
                if(tim<ts)continue;
                point.push_back(u);
                point.push_back(v);
                outLabel[u].push_back(g);
                outLabel2[v].push_back(g);
            }
            if(!edge[t].empty())
            for (it = edge[t].begin(); it != edge[t].end(); it++) {
                std::pair<long long,int> g=*it;
                if(table.find(g)!=table.end())continue;
                int u=find(g.first>>37),v=find((g.first>>12)&(33554431ll)),tim=g.second;
                if(u==v){tmpedge.push_back(g);continue;}
                if(tim<ts)continue;
                point.push_back(u);
                point.push_back(v);
                outLabel[u].push_back(g);
                outLabel2[v].push_back(g);
            }
            //only vertex connected to edges needs to be considered in running SCC
            sort(point.begin(),point.end());
            std::vector<int>:: iterator pos=std::unique(point.begin(),point.end());
            point.erase(pos,point.end());
            // int reaf=0;
            // update the edges
            edge[t]=tmpedge;

            //run the scc
            //std::cerr<<"Run the scc.\n";
            if(!markedVertices2.empty())
            for(auto u:markedVertices2){
                Vis2[u]=0;
            }
            if(!markedVertices.empty())
            for (auto u:markedVertices){
                Vis[u] = 0;
            }
            markedVertices2.clear();
            markedVertices.clear();
            top=0;
            if(!point.empty())
            for(auto g:point){
                if(!Vis[g]){
                    kosaraju1(g);
                }
            }
            if(!markedVertices.empty())
            for(auto u:markedVertices){
                Vis[u]=0;
            }
            markedVertices.clear();
            col=0;
            //if(ts==132)std::cerr<<"Go through it.\n"<<top<<'\n';
            while(top){
                //if(ts==132)std::cerr<<top<<'\n';
                int u=Sta[top];top--;
                //if(ts==132)std::cerr<<u<<' '<<top<<'\n';
                int g=find(u);
                if(Vis2[g])continue;
                col++;
                CC.clear();
                kosaraju2(g,ts);
                kosaraju4(g,g,ts);
                std::vector<std::pair<long long,int>> tmp;
                tmp.clear();
                for(auto u:CC){
                    std::vector<std::pair<long long,int>>::iterator iter;
                    for(iter=outLabel2[u].begin();iter!=outLabel2[u].end();iter++){
                        long long v=(*iter).first>>37;
                        if(find(v)!=g){
                            tmp.push_back(*iter);
                        }
                        else{
                            edge[t].push_back(*iter);
                        }
                    }
                }
                for(auto u:CC){
                    outLabel2[u].clear();
                    std::vector<std::pair<long long,int>>().swap(outLabel2[u]);
                }
                outLabel2[g]=tmp;
                tmp.clear();
                for(auto u:CC){
                    std::vector<std::pair<long long,int>>::iterator iter;
                    for(iter=outLabel[u].begin();iter!=outLabel[u].end();iter++){
                        long long v=((*iter).first>>12)&(33554431ll);
                        if(find(v)!=g){
                            tmp.push_back(*iter);
                        }
                    }
                }
                for(auto u:CC){
                    outLabel[u].clear();
                    std::vector<std::pair<long long,int>>().swap(outLabel[u]);
                }
                outLabel[g]=tmp;
                tmp.clear();
            }
            
        }
        
        //update the RES-index
        tmper=key;
        if(!key.empty())
        for(auto e: key){
            if(S.count(e)){
                S[e].second=ts;
            }
            else{
                S[e]=std::pair<int,int>(ts,ts);
            }
        }
       if(ts%100 == 0)
        putProcess(double(ts+1) / (t1+1), currentTime() - start_time);
    }
    //std::cerr<<"now?\n";
    for(auto e:S){
        int ts=e.second.first;
        int te=e.second.second;
        G[te].push_back(RES(e.first,ts));
        Chunk[te/len].push_back(RES(e.first,ts));
    }
    for(int t=0;t<=t1;t++){
        sort(G[t].begin(),G[t].end(),cmp);
    }
    for(int t=0;t<=t1/len;t++){
        sort(Chunk[t].begin(),Chunk[t].end(),cmp);
    }
    //std::cerr<<"Here?\n";
        // for(int lt=0;lt<=t1;lt++){
        //     int len=actual_time[lt].size();
        //     for(int i=len-1;i>=0;i--){
        //         if(actual_time[lt][i]<t1)break;
        //         if(actual_time[lt][i]==t1){
                    
        //             alfa.clear();
        //             std::set<std::pair<long long,int>>::iterator iter;
        //             for(iter=S[lt][i].begin();iter!=S[lt][i].end();iter++){
        //                 alfa.push_back(*iter);
        //             }
        //             G[lt].push_back(alfa);
        //         }
        //     }
        // }
    delete [] edge;
    //delete [] S;
}

OptimizedIndex::~OptimizedIndex() {
    delete [] Vis;
    delete [] f;
    delete [] Vis2;
    delete [] G;

}

unsigned long long OptimizedIndex::size() {

    unsigned long long memory = 0;
    for (int te = 0; te <= tmax; te++) {
    //     if(actual_time[ts].empty())continue;
    //     int len=actual_time[ts].size();
    //     unsigned long long sz=0;
    //     for (int te = 0; te < len; te++) {
    //         //sz += S[ts][te].size();
    //         sz+=G[ts][te].size();
    //     }
    //    // std::cerr<<sz<<' '<<ts<<'\n';
    //     memory+=sz;
        memory+=G[te].size();
    }
    // std::cout << "number of effective edges: " << cnt << std::endl;
    memory *= 32;
    return memory;

}


void OptimizedIndex::modify(TemporalGraph * Graph,int tpre,int tim){
    if(tpre==tim)return ;
    //std::cerr<<tpre<<' '<<tim<<'\n';
    unsigned long long start_time = currentTime();
    std::map<std::pair<long long,int>, std::pair<int,int> >mp;
    mp.clear();
    
    for(int i=0;i<=tim;i++){
        for(int u=0;u<n;u++){
            f[u]=u;
            Vis[u]=0;
            Vis2[u]=0;
            outLabel2[u].clear();
            outLabel[u].clear();
        }
        key.clear();
        if(i<=tpre){
            int ts=i,te=tpre;
            for (int u = 0; u < n; u++) {
                Vis[u] = 0;
                outLabel[u].clear();
                outLabel2[u].clear();
            }

            int cnt = 0;

            for(int j=ts;j<=te;j++){
                for(int now=0;now<G[j].size();now++){
                    if(G[j][now].ts<=ts){
                        std::pair<long long,int> g=G[j][now].edge;
                        long long u = (g.first >> 37), v = (g.first >> 12) & (33554431ll);
                        if(g.second>te) continue;
                        outLabel[u].push_back(g);
                        outLabel2[v].push_back(g);
                    }
                    else break;
                }
            }
            for(int u=0;u<n;u++){
                if(!Vis[u]){
                    kosaraju1(u);
                }
            }
            for(int u=0;u<n;u++)Vis[u]=0;
            col=0;
            while(top){
                int t=0;
                int u=Sta[top];top--;
                if(Vis2[u])continue;
                CC.clear();
                col++;
                kosaraju2(u,ts);
                kosaraju4(u,u,ts);
                //std::sort(CC.begin(),CC.end());
                //CurrentCC[CC[0]]=CC;
            }    
            // for (int i = 0; i <= ts; i++) {
            //     int idx = find_an_index(i, ts, te);
            //     if (idx == -1) {
            //         continue;
            //     }
            //     for (int j = idx; j >= 0; --j) {
            //         if (actual_time[i][j] < ts) {
            //             break;
            //         }
            //         for (auto g:G[i][j]) {
            //             long long tm = g.second;
            //             if (tm > te) {
            //                 continue;
            //             }
            //             long long u = (g.first >> 37), v = (g.first >> 12) & (33554431ll);
            //             outLabel[u].push_back(g);
            //             outLabel2[v].push_back(g);
            //         }
            //     }
            //     for (int j = idx+1; j < actual_time[i].size(); ++j) {
            //         if (actual_time[i][j] > te) {
            //             break;
            //         }
            //         for (auto g:G[i][j]) {
            //             long long tm = g.second;
            //             if (tm > te) {
            //                 continue;
            //             }
            //             long long u = (g.first >> 37), v = (g.first >> 12) & (33554431ll);
            //             outLabel[u].push_back(g);
            //             outLabel2[v].push_back(g);
            //         }
            //     }
            // }
            // top=0;
            // for(int u=0;u<n;u++){
            //     int g=find(u);
            //     if(!Vis[g]){
            //         kosaraju1(g);
            //     }
            // }
            // key.clear();
            // for(int u=0;u<n;u++)Vis[u]=0;
            // markedVertices.clear();
            // markedVertices2.clear();
            // col=0;
            // while(top){
            //     int u=Sta[top];top--;
            //     int g=find(u);
            //     if(!Vis2[g]){
            //         CC.clear();
            //         col++;
            //         kosaraju2(g,ts);
            //         kosaraju4(g,g,ts);
            //     }
            // }
            // for(int u=0;u<n;u++){
            //     outLabel2[u].clear();
            //     outLabel[u].clear();
            // }
            // key.clear();
        }
        for(int p=i;p<=tpre;p++){
            for(auto ed:Graph->temporal_edge[p]){
                    int u=ed.first,v=ed.second;
                    if(find(u)==find(v))continue;
                    std::pair<long long,int> g=std::pair<long long,int>((((long long)u)<<37)+(((long long)v)<<12),p);
                    outLabel[find(u)].push_back(g);
                    outLabel2[find(v)].push_back(g);
                }
        }
        for(int st=std::max(i,tpre+1);st<=tim;st++){
            std::unordered_set<int> point;
            point.clear();
            for(int i=0;i<n;i++){
                Vis[i]=0;
                Vis2[i]=0;
            }
            for(auto ed:Graph->temporal_edge[st]){
                int u=ed.first,v=ed.second;
                if(find(u)==find(v))continue;
                std::pair<long long,int> g=std::pair<long long,int>((((long long)u)<<37)+(((long long)v)<<12),st);
                point.insert(find(u));
                point.insert(find(v));
                outLabel[find(u)].push_back(g);
                outLabel2[find(v)].push_back(g);
            }
            top=0;
            col=0;
            for(auto g:point){
                if(!Vis[g]){
                    kosaraju1(g);
                }
            }
            for(int u=0;u<n;u++)Vis[u]=0;
            markedVertices.clear();
            markedVertices2.clear();
            
                while(top){
                    int u=Sta[top];top--;
                    int g=find(u);
                    if(!Vis2[g]){
                        CC.clear();
                        col++;
                        kosaraju2(g,i);
                        kosaraju4(g,g,i);
                        std::vector<std::pair<long long,int>> tmp;
                        tmp.clear();
                        for(auto u:CC){
                            std::vector<std::pair<long long,int>>::iterator iter;
                            for(iter=outLabel2[u].begin();iter!=outLabel2[u].end();iter++){
                                long long v=(*iter).first>>37;
                                if(find(v)!=g){
                                    tmp.push_back(*iter);
                                }
                            }
                        }
                        for(auto u:CC){
                            outLabel2[u].clear();
                            std::vector<std::pair<long long,int>>().swap(outLabel2[u]);
                        }
                        outLabel2[g]=tmp;
                        tmp.clear();
                        for(auto u:CC){
                            std::vector<std::pair<long long,int>>::iterator iter;
                            for(iter=outLabel[u].begin();iter!=outLabel[u].end();iter++){
                                long long v=((*iter).first>>12)&(33554431ll);
                                if(find(v)!=g){
                                    tmp.push_back(*iter);
                                }
                            }
                        }
                        for(auto u:CC){
                            outLabel[u].clear();
                            std::vector<std::pair<long long,int>>().swap(outLabel[u]);
                        }
                        outLabel[g]=tmp;
                        tmp.clear();
                    }
                }
        }
            //std::cerr<<i<<' '<<key.size()<<'\n';
            if(!key.empty()){
                for(auto g:key){
                    //std::cerr<<g.second<<'\n';
                    if(mp.count(g)){
                        std::pair<int,int> alfa=mp[g];
                        alfa.second=i;
                        mp[g]=alfa;
                    }
                    else{
                        mp[g]=std::pair<int,int>(i,i);
                    }
                }
                key.clear();
            }
        if(i%100==0)
        putProcess(double(i) / tmax, currentTime() - start_time);
        
    }
    //std::cerr<<mp.size()<<'\n';
    for(auto state:mp){
        std::pair<long long,int> g=state.first;
        std::pair<int,int> p=state.second;
        G[p.second].push_back(RES(g,p.first));
        Chunk[p.second/len].push_back(RES(g,p.first));
        // int flag=0;
        // for(int i=0;i<actual_time[p.first].size();i++){
        //     if(actual_time[p.first][i]==p.second){
        //         flag=1;
        //         G[p.first][i].push_back(g);
        //         break;
        //     }
        // }
        // if(flag==0){
        //     actual_time[p.first].push_back(0);
        //     std::vector<std::pair<long long,int> > tmp;
        //     tmp.push_back(g);
        //     G[p.first].push_back(tmp);
        //     for(int i=actual_time[p.first].size()-2;i>=0;i--){
        //         if(actual_time[p.first][i]<p.second){
        //             flag=1;
        //             actual_time[p.first][i+1]=p.second;
        //             G[p.first][i+1]=tmp;
        //             break;
        //         }
        //         else{
        //             actual_time[p.first][i+1]=actual_time[p.first][i];
        //             G[p.first][i+1]=G[p.first][i];
        //         }
        //     }
        //     if(!flag){
        //         actual_time[p.first][0]=p.second;
        //         G[p.first][0]=tmp;
        //     }
        // }
    }    
    for(int i=0;i<=tim;i++){
        sort(G[i].begin(),G[i].end(),cmp);
    }
    for(int i=0;i<=tim/len;i++){
        sort(Chunk[i].begin(),Chunk[i].end(),cmp);
    }
}
    

void OptimizedIndex::update(TemporalGraph * Graph){
    modify(Graph,t1,tmax);
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
    unsigned long long start_time = currentTime();
    while (fin >> ts >> te) {
        fout << Index->solve(vertex_num, ts, te).str() << std::endl;
        putProcess(double(++i) / query_num, currentTime() - start_time);
    }

    std::cout << "Average (per query): " << timeFormatting((currentTime() - start_time) / query_num).str() << std::endl;

}
//g++ commonfunctions.cpp temporal_graph.cpp online_search.cpp baseline.cpp divide_and_conquer.cpp optimized.cpp main.cpp -o main -O3 -std=c++11