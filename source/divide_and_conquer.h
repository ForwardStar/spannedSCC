#include "commonfunctions.h"
#include "temporal_graph.h"

class DCindex{

    private:

        int fastFind(int u);

        void fastUnion(int u, int v);
    
        int find(int ts,int u);
    
        void unioN(int ts, int u, int v, int t);
    
        void tarjanOnLeafNode(int now, int &t, int ts, int te);
        
        void tarjanOnNonLeafNode(int now, int &t);

        int *inOrder;
        int *lowestOrder;
        int *f;
        int *sz;
        bool *outOfStack;
        bool *Vis;
        int tot = 0;

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