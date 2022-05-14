#include "commonfunctions.h"
#include "temporal_graph.h"

class OnlineSearch {

    private:

        int *dfsOrder;
        int *lowestOrder;
        bool *outOfStack;
        bool *Vis;
        std::stack<int> Stack;
        std::vector<std::vector<int>> AllSCC;
        TemporalGraph * G;

        void tarjan(int now, int &t);
    
    public:

        std::stringstream onlineSearch(TemporalGraph * Graph, int ts, int te);

        OnlineSearch(int n);
        ~OnlineSearch();

};

void online(TemporalGraph * Graph, char * query_file, char * output_file);