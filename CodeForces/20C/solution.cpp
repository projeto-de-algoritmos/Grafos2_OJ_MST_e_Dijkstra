#include <bits/stdc++.h>

//#define endl '\n'
#define int long long
#define pb push_back
#define ff first
#define ss second
#define all(x) (x).begin(), (x).end()
#define ll long long
#define ii pair<int,int>
#define vi vector<int>
#define vll vector<ll>
#define vii vector<ii>
 
const int INF = 0x3f3f3f3f;
const ll  LINF = 0x3f3f3f3f3f3f3f3fLL;
const int maxn = 1e5+10;
const int mod = 1e9+7;

using namespace std;

vector<ii> graph[maxn];

vector<int> dijkstra(int s, int e){

    // heap para executar o algoritmo
    priority_queue<ii,vii, greater<ii>> pq;

    // vetor em que será possível acessar a distância para o nó i em O(1)
    vector<int> dist(e+1,LINF);

    // nó pai do nó i
    vector<int> parent(e+1);

    pq.push({0,s});
    dist[s] = 0;

    // dijkstra padrão
    while(!pq.empty()){

        auto [w, v] = pq.top();
        pq.pop();

        if(w > dist[v]) continue;

        for(auto &[n_v, n_w] : graph[v]){
            if(dist[n_v] > w + n_w){
                dist[n_v] = w + n_w;
                pq.push({dist[n_v], n_v});
                parent[n_v] = v;
            }
        }
    }

    vector<int> ans;

    // Não é possível chegar em e a partir do nó s
    if(dist[e] == LINF){
        ans.pb(-1ll);
    }
    else{

        // recupera o caminho mais curto subindo do nó e até o nó s
        // lembrando que o pai nó 1 é o 0
        while(e != 0){
            ans.pb(e);
            e = parent[e];
        }

        // inverte a ordem do vetor pois ele está do nó e até o s
        // e o problema quer do nó s até o nó e
        reverse(all(ans));
    }

    return ans;

}

void solve(){
	
    int n, m;

    cin >> n >> m;

    for(int i=0;i<m;++i){
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].pb({v,w});
        graph[v].pb({u,w});
    }

    vector<int> ans = dijkstra(1,n);

    for(auto x : ans){
        cout << x << " ";
    }

    cout << endl;

}

int32_t main(){
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	int t=1;
	//cin>>t;
	while(t--){
		solve();
	}
	return 0;
}
