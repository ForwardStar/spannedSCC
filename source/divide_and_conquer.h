#include "commonfunctions.h"
#include "temporal_graph.h"

class DCindex{
    private:

    int eefind(int u);
    int efind(int u);
    int find(int ts,int u);
    void unioN(int ts, int u, int v, int t);
    void tarjan(int now, int t, int ts, int te);
    void tarjan2(int now,int t);
    void div(int ts,int l,int r);
    void clear();
    int *inOrder;
    int *lowestOrder;
    int *f;
    int *sz;
    bool *outOfStack;
    bool *Vis;
    int tot=0;
    std::unordered_set<long long> *edge;
    std::unordered_set<int> *outLabel;

    std::stack<int> Stack;

    public:

    int n, m, tmax;

    int **L;
    int **T;
    int *size;

    std::stringstream solve(int n, int ts, int te);
    
    DCindex() {}
    DCindex(TemporalGraph * Graph);
    ~DCindex();
};

void DC(DCindex *Index, int vertex_num, char * query_file, char * output_file);