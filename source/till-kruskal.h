#include "commonfunctions.h"
#include "temporal_graph.h"
#include "till.h"

class TILL_Kruskal {

    private:

        // find(ts, u, id): find the label of u at time t(ts, u, id) with start time ts.
        int find(int ts, int u);
    
        // unioN(ts, u, v, t): perform the union operation on u and v at time t with start time ts.
        void unioN(int ts, int u, int v, int t);

        void update(int u, int v, int ts, int te, TILL * TILL_Index);

        std::unordered_set<int> isolated_vertices;

    public:

        // n, m, tmax: graph information.
        int n, m, tmax;
        
        int **L;
        int **T;
        int *size;

        std::stringstream solve(int n, int ts, int te);

        TILL_Kruskal() {}
        TILL_Kruskal(TemporalGraph * Graph, TILL * TILL_Index);
        ~TILL_Kruskal();

};

void TILL_kruskal(TILL_Kruskal * Index, int vertex_num, char * query_file, char * output_file);