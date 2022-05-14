#include "commonfunctions.h"
#include "temporal_graph.h"
#include "online_search.h"

TemporalGraph * build(char * argv[]) {
    std::cout << "Building graph..." << std::endl;
    int build_graph_start_time = time(NULL);
    TemporalGraph * Graph = new TemporalGraph(argv[1], (char *)"Directed");
    int build_graph_end_time = time(NULL);
    std::cout << "Build graph success in " << timeFormatting(difftime(build_graph_end_time, build_graph_start_time)).str() << std::endl;
    std::cout << "n = " << Graph->numOfVertices() << ", m = " << Graph->numOfEdges() << ", tmax = " << Graph->tmax << std::endl;
    return Graph;
}

int main(int argc, char * argv[]) {

    std::ios::sync_with_stdio(false);

    int start_time = time(NULL);

    if (argc != 5) {
        std::cout << "Parameter error. Please check the readme file." << std::endl;
    	return 0;
    }

    TemporalGraph * Graph = build(argv);
    int vertex_num = Graph->numOfVertices();

    if (std::strcmp(argv[4], "Online") == 0) {
        std::cout << "Running online search..." << std::endl;
        int online_search_start_time = time(NULL);
        online(Graph, argv[2], argv[3]);
        int online_search_end_time = time(NULL);
        std::cout << "Online search completed in " << timeFormatting(difftime(online_search_end_time, online_search_start_time)).str() << std::endl;
        delete Graph;
    }

    int end_time = time(NULL);
    std::cout << "Program finished in " << timeFormatting(difftime(end_time, start_time)).str() << std::endl;

    return 0;

}