#include "commonfunctions.h"
#include "temporal_graph.h"

int *dfsOrder;
int *lowestOrder;
bool *outOfStack;
bool *Vis;
std::stack<int> Stack;
std::vector<std::vector<int>> AllSCC;
TemporalGraph * G;

void online(TemporalGraph * Graph, char * query_file, char * output_file);