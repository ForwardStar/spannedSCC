#include "commonfunctions.h"
#include "temporal_graph.h"

class DCIndex {

    private:

        int fastFind(int u);

        void fastUnion(int u, int v);
    
        int find(int ts,int u);
        bool hashfind(int u,int v);
        void unioN(int ts, int u, int v, int t);
        
        void tarjan(int now, int &t, int &te);

        int *inOrder;
        int *outOrder;
        int *lowestOrder;
        int *f;
        int *sz;
        bool *outOfStack;
        bool *Vis;
        bool *hasTarjaned;

        std::list<std::pair<int, int>> *edges;
        std::vector<std::pair<int, int>> *toBeMerged;
        std::vector<int> **outLabel;

        std::stack<int> Stack;

    public:

        int n, m, tmax;

        int **L;
        int **T;
        int *size;

        std::stringstream solve(int n, int ts, int te);
        
        DCIndex() {}
        DCIndex(TemporalGraph * Graph);
        ~DCIndex();

};

void DC(DCIndex *Index, int vertex_num, char * query_file, char * output_file);