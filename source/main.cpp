#include "commonfunctions.h"
#include "temporal_graph.h"
#include "online_search.h"
#include "baseline.h"
#include "divide_and_conquer.h"
#include "optimized.h"

TemporalGraph * build(char * argv[]) {

    std::cout << "Building graph..." << std::endl;
    unsigned long long build_graph_start_time = currentTime();
    TemporalGraph * Graph = new TemporalGraph(argv[1], (char *)"Directed", 1);
    unsigned long long build_graph_end_time = currentTime();
    std::cout << "Build graph success in " << timeFormatting(build_graph_end_time - build_graph_start_time).str() << std::endl;
    std::cout << "n = " << Graph->numOfVertices() << ", m = " << Graph->numOfEdges() << ", tmax = " << Graph->tmax << ", size = " << Graph->size() << " bytes" << std::endl;
    return Graph;
    
}

int main(int argc, char * argv[]) {

    std::ios::sync_with_stdio(false);

    unsigned long long start_time = currentTime();

    if (argc != 5) {
        std::cout << "Parameter error. Please check the readme file." << std::endl;
    	return 0;
    }

    TemporalGraph * Graph = build(argv);
    // Graph->shrink_to_fit();
    int vertex_num = Graph->numOfVertices();

    if (std::strcmp(argv[4], "Online") == 0) {
        std::cout << "Running online search..." << std::endl;
        unsigned long long online_search_start_time = currentTime();
        online(Graph, argv[2], argv[3]);
        unsigned long long online_search_end_time = currentTime();
        std::cout << "Online search completed in " << timeFormatting(online_search_end_time - online_search_start_time).str() << std::endl;
        delete Graph;
    }

    if (std::strcmp(argv[4], "Baseline") == 0) {
        std::cout << "Running baseline..." << std::endl;
        std::cout << "Constructing the index structure..." << std::endl;
        unsigned long long index_construction_start_time = currentTime();
        BaselineIndex *Index = new BaselineIndex(Graph);
        unsigned long long index_construction_end_time = currentTime();
        std::cout << "Index construction completed in " << timeFormatting(index_construction_end_time - index_construction_start_time).str() << std::endl;
        std::cout << "Index cost " << Index->size() << " bytes" << std::endl;
        delete Graph;
        std::cout << "Solving queries..." << std::endl;
        unsigned long long query_start_time = currentTime();
        baseline(Index, vertex_num, argv[2], argv[3]);
        unsigned long long query_end_time = currentTime();
        std::cout << "Query completed in " << timeFormatting(query_end_time - query_start_time).str() << std::endl;
        std::cout << "Baseline completed!" << std::endl;
    }

    if (std::strcmp(argv[4], "DC") == 0) {
        std::cout << "Running DC..." << std::endl;
        std::cout << "Constructing the index structure..." << std::endl;
        unsigned long long index_construction_start_time = currentTime();
        DCIndex *Index = new DCIndex(Graph);
        unsigned long long index_construction_end_time = currentTime();
        std::cout << "Index construction completed in " << timeFormatting(index_construction_end_time - index_construction_start_time).str() << std::endl;
        delete Graph;
        std::cout << "Solving queries..." << std::endl;
        unsigned long long query_start_time = currentTime();
        DC(Index, vertex_num, argv[2], argv[3]);
        unsigned long long query_end_time = currentTime();
        std::cout << "Query completed in " << timeFormatting(query_end_time - query_start_time).str() << std::endl;
        std::cout << "DC completed!" << std::endl;
    }

    if (std::strcmp(argv[4], "Optimized") == 0) {
        std::cout << "Running optimized..." << std::endl;
        std::cout << "Constructing the index structure..." << std::endl;
        unsigned long long index_construction_start_time = currentTime();
        OptimizedIndex *Index = new OptimizedIndex(Graph);
        unsigned long long index_construction_end_time = currentTime();
        std::cout << "Index construction completed in " << timeFormatting(index_construction_end_time - index_construction_start_time).str() << std::endl;
        std::cout << "Index cost " << Index->size() << " bytes" << std::endl;
        delete Graph;
        std::cout << "Solving queries..." << std::endl;
        unsigned long long query_start_time = currentTime();
        optimized(Index, vertex_num, argv[2], argv[3]);
        unsigned long long query_end_time = currentTime();
        std::cout << "Query completed in " << timeFormatting(query_end_time - query_start_time).str() << std::endl;
        std::cout << "Optimized completed!" << std::endl;
    }
    unsigned long long end_time = currentTime();
    std::cout << "Program finished in " << timeFormatting(difftime(end_time, start_time)).str() << std::endl;

    return 0;

}