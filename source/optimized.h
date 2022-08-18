#include "commonfunctions.h"
#include "temporal_graph.h"

class OptimizedIndex {

    private:

        // find(ts, u, id): find the label of u at time t(ts, u, id) with start time ts.
        int find(int u);
    
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

        std::vector<long long> *outLabel, *outLabel2;
        std::set<long long>  **S;
        std::set<long long> *edge;
        std::vector<long long> **G;
        std::stack<int> Stack;
        std::vector<int> CC;

    public:

        // n, m, tmax: graph information.
        int n, m, tmax;
        
        int *L;
        int *T;
        int *size;

        std::stringstream solve(int n, int ts, int te);

        OptimizedIndex() {}
        OptimizedIndex(TemporalGraph * Graph);
        ~OptimizedIndex();

};

void optimized(OptimizedIndex * Index, int vertex_num, char * query_file, char * output_file);
