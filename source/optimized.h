#include "commonfunctions.h"
#include "temporal_graph.h"

class OptimizedIndex {

    private:

        // find(ts, u, id): find the label of u at time t(ts, u, id) with start time ts.
        int find(int u);

        int find_an_index(int t, int ts, int te);
    
        // unioN(ts, u, v, t): perform the union operation on u and v at time t with start time ts.

        void kosaraju1(int now);
        void kosaraju3(int now);
        void kosaraju5(int now);
        void kosaraju2(int now, int ts);
        void kosaraju4(int now, int ori,int ts);
        int top,col;
        int *inOrder;
        int *outOrder;
        int *lowestOrder;
        int *Sta;
        int *Vis,*Vis2;
        int *f;

        std::vector<int> markedVertices;
        std::vector<int> markedVertices2;
        std::set<std::pair<long long,int>> key;
        std::vector<std::pair<long long,int>> *outLabel, *outLabel2;
        std::vector<std::set<std::pair<long long,int>>>  *S;
        std::vector<int> *actual_time;
        std::vector<int> *actual_time_temporal;
        std::vector<std::pair<long long,int>> *edge;
        std::vector<std::pair<long long,int>> tmpedge;
        std::vector<std::vector<std::pair<long long,int>>> *G;
        std::vector<std::vector<std::pair<long long,int>>> *G_temporal;

        std::stack<int> Stack;
        std::vector<int> CC;
        std::vector<std::pair<long long,int>> *newedge;
        std::set<std::pair<long long,int>> beta;
        std::set<std::pair<long long,int>> tmper;
        std::vector<std::pair<long long,int>> alfa;

    public:

        // n, m, tmax: graph information.
        int n, m, tmax, t1;

        std::stringstream solve(int n, int ts, int te);
        void update(TemporalGraph * Graph);
        void modify(TemporalGraph * Graph,int tpre,int tim);
        OptimizedIndex() {}
        OptimizedIndex(TemporalGraph * Graph, double t_fraction);
        ~OptimizedIndex();

        unsigned long long size();

};

void optimized(OptimizedIndex * Index, int vertex_num, char * query_file, char * output_file);
