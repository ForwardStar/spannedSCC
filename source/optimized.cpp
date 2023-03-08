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
    markedVertices.push_back(now);
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
    for (int i = 0; i <= ts; i++) {
        int idx = find_an_index(i, ts, te);
        if (idx == -1) {
            continue;
        }
        for (int j = idx; j >= 0; --j) {
            if (actual_time[i][j] < ts) {
                break;
            }
            for (auto g:G[i][j]) {
                long long tm = g.second;
                if (tm > te) {
                    continue;
                }
                cnt++;
                long long u = (g.first >> 37), v = (g.first >> 12) & (33554431ll);
                outLabel[u].push_back(g);
                outLabel2[v].push_back(g);
            }
        }
        for (int j = idx+1; j < actual_time[i].size(); ++j) {
            if (actual_time[i][j] > te) {
                break;
            }
            for (auto g:G[i][j]) {
                long long tm = g.second;
                if (tm > te) {
                    continue;
                }
                cnt++;
                long long u = (g.first >> 37), v = (g.first >> 12) & (33554431ll);
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

OptimizedIndex::OptimizedIndex(TemporalGraph * Graph, double t_fraction) {
    
    unsigned long long start_time = currentTime();

    n = Graph->numOfVertices();
    m = Graph->numOfEdges();
    tmax = Graph->tmax;
    Sta = new int[n];
    Vis = new int[n];
    Vis2 = new int[n];
    f = new int[n];
    S = new std::vector<std::set<std::pair<long long,int>>> [tmax+1]();
    edge = new std::vector<std::pair<long long,int>> [tmax+1]();
    newedge = new std::vector<std::pair<long long,int>> [tmax+1]();
    G = new std::vector<std::vector<std::pair<long long,int>>> [tmax+1]();
    actual_time= new std::vector<int> [tmax+1]();
    outLabel = new std::vector<std::pair<long long,int>>[n]();
    outLabel2 = new std::vector<std::pair<long long,int>>[n]();
    top=0;
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
            std::unordered_set<int> point;
            point.clear();
            std::vector<std::pair<long long,int>>::iterator it;
            tmpedge.clear();
            for(it = newedge[t].begin();it!=newedge[t].end();it++){
                std::pair<long long,int> g=*it;
                int u=find(g.first>>37),v=find((g.first>>12)&(33554431ll)),tim=g.second;
                if(u==v){tmpedge.push_back(g);continue;}
                if(tim<ts)continue;
                point.insert(u);
                point.insert(v);
                outLabel[u].push_back(g);
                outLabel2[v].push_back(g);
            }
            for (it = edge[t].begin(); it != edge[t].end(); it++) {
                std::pair<long long,int> g=*it;
                if(table.find(g)!=table.end())continue;
                int u=find(g.first>>37),v=find((g.first>>12)&(33554431ll)),tim=g.second;
                if(u==v){tmpedge.push_back(g);continue;}
                if(tim<ts)continue;
                point.insert(u);
                point.insert(v);
                outLabel[u].push_back(g);
                outLabel2[v].push_back(g);
            }
            edge[t]=tmpedge;
            for(auto u:markedVertices2){
                Vis2[u]=0;
            }
            for (auto u:markedVertices){
                Vis[u] = 0;
            }
            markedVertices2.clear();
            markedVertices.clear();
            top=0;
            for(auto g:point){
                if(!Vis[g]){
                    kosaraju1(g);
                }
            }
            for(auto u:markedVertices){
                Vis[u]=0;
            }
            markedVertices.clear();
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
                            //std::cerr<<u<<' '<<v<<' '<<((*iter)&4095)<<' '<<t<<'\n';
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
                //std::vector<long long>().swap(tmp);
            }
            
        }
        
        tmper=key;
        for(int lt=0;lt<ts;lt++){
            int len=actual_time[lt].size();
            if(len==0){
                continue;
            }
            int tag=0;
            if(actual_time[lt][len-1]!=ts-1){
                //std::cerr<<lt<<' '<<ts<<' '<<actual_time[lt][len-1]<<'\n';
                continue;
            }
            std::set<std::pair<long long,int>>::iterator iter;
            std::set<std::pair<long long,int>>::iterator g;
            
            beta.clear();
            //int cnt=0;
            for(iter = S[lt][len-1].begin();iter!=S[lt][len-1].end();){
                
                if(tmper.find(*iter)!=tmper.end()){
                    //cnt++;
                    //std::cerr<<(*iter).first<<' '<<(*iter).second<<'\n';
                    
                    tmper.erase(*iter);
                    beta.insert(*iter);
                    S[lt][len-1].erase(iter++);
                }
                else{
                    iter++;
                    tag=1;
                }
            }
            //std::cerr<<cnt<<'\n';
            if(!tag){
                S[lt][len-1]=beta;
                actual_time[lt][len-1]=ts;
            }
            else{
                //std::cerr<<beta.size()<<'\n';
                S[lt].push_back(beta);
                actual_time[lt].push_back(ts);
            }
        }
        for(int lt=0;lt<ts;lt++){
            //std::cerr<<lt<<' '<<ts<<' '<<S[lt][ts-1].size()<<'\n';
            int len=actual_time[lt].size();
            for(int i=len-1;i>=0;i--){
                if(actual_time[lt][i]<ts-1)break;
                if(actual_time[lt][i]==ts-1){
                    alfa.clear();
                    std::set<std::pair<long long,int>>::iterator iter;
                    for(iter=S[lt][i].begin();iter!=S[lt][i].end();iter++){
                        alfa.push_back(*iter);
                    }
                    G[lt].push_back(alfa);
                    std::vector<std::pair<long long,int>>().swap(alfa);
                }
            }
        }
        if(!tmper.empty()){
            S[ts].push_back(tmper);
            actual_time[ts].push_back(ts);
        }
       // std::cerr<<size()<<'\n';
       //std::cout<<ts<<' '<<size()<<'\n';
        putProcess(double(ts) / t1, currentTime() - start_time);
        //if(ts==1)break;
    }
        for(int lt=0;lt<=t1;lt++){
            //std::cerr<<lt<<' '<<ts<<' '<<S[lt][ts-1].size()<<'\n';
            int len=actual_time[lt].size();
            for(int i=len-1;i>=0;i--){
                if(actual_time[lt][i]<t1)break;
                if(actual_time[lt][i]==t1){
                    
                    alfa.clear();
                    std::set<std::pair<long long,int>>::iterator iter;
                    for(iter=S[lt][i].begin();iter!=S[lt][i].end();iter++){
                        alfa.push_back(*iter);
                    }
                    G[lt].push_back(alfa);
                }
            }
        }
    delete [] edge;
    delete [] S;
}

OptimizedIndex::~OptimizedIndex() {
    delete [] Vis;
    delete [] f;
    delete [] Vis2;
    delete [] G;

}

unsigned long long OptimizedIndex::size() {

    unsigned long long memory = 0;
    for (int ts = 0; ts <= tmax; ts++) {
        if(actual_time[ts].empty())continue;
        int len=actual_time[ts].size();
        unsigned long long sz=0;
        for (int te = 0; te < len; te++) {
            //sz += S[ts][te].size();
            sz+=G[ts][te].size();
        }
       // std::cerr<<sz<<' '<<ts<<'\n';
        memory+=sz;
    }
    // std::cout << "number of effective edges: " << cnt << std::endl;
    memory *= 12;
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
            for (int i = 0; i <= ts; i++) {
                int idx = find_an_index(i, ts, te);
                if (idx == -1) {
                    continue;
                }
                for (int j = idx; j >= 0; --j) {
                    if (actual_time[i][j] < ts) {
                        break;
                    }
                    for (auto g:G[i][j]) {
                        long long tm = g.second;
                        if (tm > te) {
                            continue;
                        }
                        long long u = (g.first >> 37), v = (g.first >> 12) & (33554431ll);
                        outLabel[u].push_back(g);
                        outLabel2[v].push_back(g);
                    }
                }
                for (int j = idx+1; j < actual_time[i].size(); ++j) {
                    if (actual_time[i][j] > te) {
                        break;
                    }
                    for (auto g:G[i][j]) {
                        long long tm = g.second;
                        if (tm > te) {
                            continue;
                        }
                        long long u = (g.first >> 37), v = (g.first >> 12) & (33554431ll);
                        outLabel[u].push_back(g);
                        outLabel2[v].push_back(g);
                    }
                }
            }
            top=0;
            for(int u=0;u<n;u++){
                int g=find(u);
                if(!Vis[g]){
                    kosaraju1(g);
                }
            }
            key.clear();
            for(int u=0;u<n;u++)Vis[u]=0;
            markedVertices.clear();
            markedVertices2.clear();
            col=0;
            while(top){
                int u=Sta[top];top--;
                int g=find(u);
                if(!Vis2[g]){
                    CC.clear();
                    col++;
                    kosaraju2(g,ts);
                    kosaraju4(g,g,ts);
                }
            }
            for(int u=0;u<n;u++){
                outLabel2[u].clear();
                outLabel[u].clear();
            }
            key.clear();
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
        putProcess(double(i) / tmax, currentTime() - start_time);
        
    }
    //std::cerr<<mp.size()<<'\n';
    for(auto state:mp){
        std::pair<long long,int> g=state.first;
        std::pair<int,int> p=state.second;
        int flag=0;
        for(int i=0;i<actual_time[p.first].size();i++){
            if(actual_time[p.first][i]==p.second){
                flag=1;
                G[p.first][i].push_back(g);
                break;
            }
        }
        if(flag==0){
            actual_time[p.first].push_back(0);
            std::vector<std::pair<long long,int> > tmp;
            tmp.push_back(g);
            G[p.first].push_back(tmp);
            for(int i=actual_time[p.first].size()-2;i>=0;i--){
                if(actual_time[p.first][i]<p.second){
                    flag=1;
                    actual_time[p.first][i+1]=p.second;
                    G[p.first][i+1]=tmp;
                    break;
                }
                else{
                    actual_time[p.first][i+1]=actual_time[p.first][i];
                    G[p.first][i+1]=G[p.first][i];
                }
            }
            if(!flag){
                actual_time[p.first][0]=p.second;
                G[p.first][0]=tmp;
            }
        }
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