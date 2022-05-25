#include "online_search.h"

bool cmp(std::vector<int> i, std::vector<int> j) {

    return i[0] < j[0];

}

void OnlineSearch::tarjan(int now, int &t) {

    dfsOrder[now] = ++t;
    lowestOrder[now] = t;
    Vis[now] = true;
    Stack.push(now);

    TemporalGraph::Edge * edge = G->getHeadEdge(now);
    
    while (edge) {
        if (!Vis[edge->to]) {
            tarjan(edge->to, t);
        }
        if (!outOfStack[edge->to]) {
            lowestOrder[now] = std::min(lowestOrder[now], lowestOrder[edge->to]);
        }
        edge = edge->next;
    }

    if (dfsOrder[now] == lowestOrder[now]) {
        std::vector<int> CurrentSCC;
        while (Stack.top() != now) {
            outOfStack[Stack.top()] = true;
            CurrentSCC.push_back(Stack.top());
            Stack.pop();
        }
        outOfStack[Stack.top()] = true;
        CurrentSCC.push_back(Stack.top());
        Stack.pop();
        std::sort(CurrentSCC.begin(), CurrentSCC.end());
        AllSCC.push_back(CurrentSCC);
    }

}

std::stringstream OnlineSearch::onlineSearch(TemporalGraph * Graph, int ts, int te) {

    int t = 0;
    std::stringstream Ans;
    G = new TemporalGraph(Graph, ts, te);

    for (int u = 0; u < Graph->numOfVertices(); ++u) {
        outOfStack[u] = false;
        Vis[u] = false;
    }
    
    Ans << "The spanned strongly connected components in [" << ts << ", " << te << "] are:\n";
    for (int u = 0; u < Graph->numOfVertices(); ++u) {
        if (!Vis[u]) {
            tarjan(u, t);
        }
    }
    
    std::sort(AllSCC.begin(), AllSCC.end(), cmp);

    std::vector<std::vector<int>>::iterator it;
    for (it = AllSCC.begin(); it != AllSCC.end(); it++) {
        Ans << "{ ";
        std::vector<int>::iterator it1;
        for (it1 = it->begin(); it1 != it->end(); it1++) {
            Ans << *it1 << " ";
        }
        Ans << "}\n";
        it->clear();
    }

    AllSCC.clear();

    delete G;

    return Ans;

}

OnlineSearch::OnlineSearch(int n) {

    dfsOrder = new int[n];
    lowestOrder = new int[n];
    outOfStack = new bool[n];
    Vis = new bool[n];

}

OnlineSearch::~OnlineSearch() {

    delete [] dfsOrder;
    delete [] lowestOrder;
    delete [] outOfStack;
    delete [] Vis;

}

void online(TemporalGraph * Graph, char * query_file, char * output_file) {

    OnlineSearch *solver = new OnlineSearch(Graph->numOfVertices());
    
    int ts, te;
    int query_num = 0;
    std::ifstream fin(query_file);
    std::ofstream fout(output_file);

    while (fin >> ts >> te) {
        ++query_num;
    }

    fin = std::ifstream(query_file);

    int i = 0;
    int start_time = time(NULL);
    while (fin >> ts >> te) {
        // Perform SCC Tarjan search
        fout << solver->onlineSearch(Graph, ts, te).str() << std::endl;
        putProcess(double(++i) / query_num, difftime(time(NULL), start_time));
    }

    delete solver;

    std::cout << "Average: " << timeFormatting(difftime(time(NULL), start_time) / query_num).str() << std::endl;

}