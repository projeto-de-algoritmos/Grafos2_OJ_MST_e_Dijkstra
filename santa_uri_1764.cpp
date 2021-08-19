#include <bits/stdc++.h>
using namespace std;

class DSU {
private:
    vector<int> parent, rank;
    int N;
public:
    DSU(int N) {
        this->N = N;
        parent = vector<int>(N);
        iota(parent.begin(), parent.end(), 0);
        rank = vector<int>(N, 1);
    }
    int find(int x) {
        return parent[x] == x ? x : find(parent[x]); 
    }
    bool isSameSet(int x, int y) {
        return find(x) == find(y);
    }
    void unionSet(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y)
            return;
        if (rank[x] > rank[y]) swap(x, y);
        parent[x] = y;
        rank[y] += rank[x];
    }
};

typedef struct Edge {
    int u, v;
    int w;
} Edge;

bool cmp(const Edge& a, const Edge& b) {
    return a.w < b.w;
}

void kruskal(vector<Edge>& edges, int M, int & mst_sum) {

    int mst = 0;

    DSU * dsu = new DSU(M);

    for (auto e : edges) { 
        if (!dsu->isSameSet(e.u, e.v)) {
            dsu->unionSet(e.u, e.v);
            mst += e.w;
        }   
    }
    
    delete dsu;
    mst_sum = mst;
}

int main() {
    int M, N;

    while(cin >> M >> N, M && N) {
        vector<Edge> edges;
        int mst_sum = 0;
        int X, Y, Z;
        for (int i = 0; i < N; i++) {
            cin >> X >> Y >> Z;
            edges.push_back({X, Y, Z});
        }
        sort(edges.begin(), edges.end(), cmp);
        kruskal(edges, M, mst_sum);
        cout << mst_sum << endl;
    }
    
    return 0; 
}