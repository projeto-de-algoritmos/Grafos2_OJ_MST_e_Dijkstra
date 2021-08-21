#include <bits/stdc++.h>
using namespace std;

typedef struct Edge {
    int u, v;
    int w;
} Edge;

class Graph {
private:
    int V;
    int E;
    vector<vector<Edge>> adj;

public:
    Graph(int V, int E) {
        this->V = V;
        this->E = E;
        adj = vector<vector<Edge>>(V);
    }

    void addEdge(Edge e) {
        adj[e.u].push_back(e);
        adj[e.v].push_back(Edge{e.v, e.u, e.w});
    }

    vector<Edge> getAdj(int u) {
        return adj[u];
    }

    Edge getEdge(int u, int v) {
        for (Edge e : adj[u]) {
            if (e.v == v) {
                return e;
            }
        }
        return Edge{-1, -1, -1};
    }

    int getVertices() {
        return V;
    }

    int getEdges() {
        return E;
    }
};

bool operator<(const Edge& lhs, const Edge& rhs) {
    return lhs.w > rhs.w;
}

int shortestPath(Graph * g, int s, int t, int C) {
    auto compare = [&](Edge lhs, Edge rhs) {
        if(lhs.w == rhs.w) {
            if (lhs.u < C && rhs.u < C) {
                return lhs.u < rhs.u;
            }

            if (lhs.u < C) {
                return true;   
            }

            if (rhs.u < C) {
                return false;
            }
        }
        return lhs.w > rhs.w;
    };

    priority_queue<Edge, vector<Edge>, decltype(compare)> pq(compare);
    vector<int> dist(g->getVertices(), INT_MAX);
    vector<bool> visited(g->getVertices(), false);
    
    pq.push(Edge{s, s, 0});
    dist[s] = 0;

    while (!pq.empty()) {
        Edge e = pq.top();
        pq.pop();

        if (visited[e.u]) continue;
        visited[e.u] = true;

        for (Edge edge : g->getAdj(e.u)) {
            if (visited[edge.v]) continue;
            if (e.u < C && e.u + 1 != edge.v) 
                continue;
            
            if (dist[edge.v] > dist[e.u] + edge.w) {
                dist[edge.v] = dist[e.u] + edge.w;
                pq.push(Edge{edge.v, edge.v, dist[edge.v]});
            }
        }
    }

    return dist[t];
}
    

int main() {
    int M, N, C, K;
    while(cin >> N >> M >> C >> K, N != 0 && M != 0 && C != 0 && K != 0) {
        Graph *G = new Graph(N, M);
        for (int i = 0; i < M; i++) {
            int u, v, w;
            cin >> u >> v >> w;
            Edge e = {u, v, w};
            G->addEdge(e);
        }
        
        cout << shortestPath(G, K, C-1, C) << endl;
        delete G;
    }
    
    return 0; 
}