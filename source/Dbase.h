#include "commonfunctions.h"
#include "temporal_graph.h"

class DifferentBaseIndex {

    private:

        // find(ts, u, id): find the label of u at time t(ts, u, id) with start time ts.
        int find(int u);
    
        // unioN(ts, u, v, t): perform the union operation on u and v at time t with start time ts.
        void unioN(int ts, int u, int v, int t);

        void kosaraju1(int now, int &t);
        void kosaraju3(int now, int &t);
        void kosaraju2(int now, int &t, int ori, int ts);
        int top;
        int *inOrder;
        int *outOrder;
        int *lowestOrder;
        int *Sta;
        bool *Vis;

        std::unordered_set<long long> *outLabel, *outLabel2;
        std::unordered_set<long long>  **S;
        std::stack<int> Stack;
        std::vector<int> CC;

    public:

        // n, m, tmax: graph information.
        int n, m, tmax;
        
        int *L;
        int *T;
        int *size;

        std::stringstream solve(int n, int ts, int te);

        DifferentBaseIndex() {}
        DifferentBaseIndex(TemporalGraph * Graph);
        ~DifferentBaseIndex();

};

void Dbase(DifferentBaseIndex * Index, int vertex_num, char * query_file, char * output_file);
