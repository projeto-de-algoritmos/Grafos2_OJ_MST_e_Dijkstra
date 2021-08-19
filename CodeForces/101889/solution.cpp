#include <bits/stdc++.h>

//#define endl '\n'
//#define int long long
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

// Constante que é um valor aproximado do Log2 do maior valor que 
// o input pode assumir.
const int LOG = 22;

using namespace std;

// Lista de adjacência para representar a árvore geradora mínima
vector<vector<ii>> mst;

// Matriz que irá salvar os "pais" de um determinado nó, será utilizado
// para realizar a subida na árvore por meio do algortimo de binary lifting
vector<vector<ii>> up;

// Vetor que salva a profundidade de um determinado nó na árvore geradora mínima
// sendo o nível 0 correspondente a raíz da árvore, no caso a fixei sendo o nó
// de número 1
vi deepth;

struct DSU { // DSU pra calcular a MST através do algoritmo de Kruskall

	vi parent, rank;

	DSU(int N){
		parent = vi(N);
		rank = vi(N,1);
		iota(all(parent),0); // preenche o vetor parent de forma que parent[i] = i
	}

	int find_set(int x){
		return parent[x] == x ? x : find_set(parent[x]); 
	}

	void union_set(int a,int b){
		a = find_set(a);
		b = find_set(b);
		if(a == b) return;
		if(rank[a] > rank[b]) swap(a,b);
		parent[a] = b;
		rank[b] += rank[a];
	}

};

struct Edge{ // Representando as arestas do grafo dado
	int u, v, w;
};

// u -> nó atual
// p -> nó pai do nó atual
// c -> peso da aresta que leva ao nó u
// d -> profundidade do nó atual na MST
void dfs(int u, int p, int c, int d){

	deepth[u] = d;


	up[u][0] = {p, c};

	// Calcula o nó que está 2^i níveis acima do nó atual na MST
	// essa técninca é conhecida como binary lifting
	// o primeiro valor do pair salva qual é este nó
	// e o segundo salva qual é o maior peso encontrado entre as arestas
	// desde o nó u até seu (2^i)-ésimo pai
	for(int i=1;i<=LOG;++i){
		int nxt = up[u][i-1].ff;
		int c = up[u][i-1].ss;
		up[u][i].ff = up[nxt][i-1].ff;
		up[u][i].ss = max(c, up[nxt][i-1].ss);
	}

	for(auto &[v, w]: mst[u]){

		if(v != p) 
			dfs(v, u, w, d+1);

	}

}

// função que recupera qual o custo da aresta mais cara entre os nós a e b
int get_max_weight(int a, int b){

	// queremos que o nó b seja o que está mais abaixo na árvore
	if(deepth[a] > deepth[b])
		swap(a,b);
	
	// diferença entre os níveis do nó a e o nó b
	int d_diff = deepth[b] - deepth[a];

	int ret = 0;

	while(d_diff > 0){

		// maior potência de 2 que b consegue subir antes de alcançar a mesma
		// profundidade que o nó a
		int log = log2(d_diff);
        ret = max(ret, up[b][log].ss);
 
		// faz b ser igual ao (2^log)-ésimo nó acima dele
        b = up[b][log].ff; 
 
		// como b subiu 2^log diminui isso da diferença entre a e b		
        d_diff -= (1 << log);

	}

	while (a != b) {

		// como a e b estão em um mesmo nível na árvore não faz diferença usar
		// a profundidade a ou b aqui
        int i = log2(deepth[b]);
 
		// procura o primeiro nó acima de a e b que é diferente do LCA de ambos
        while (i > 0 && up[a][i].ff == up[b][i].ff)
            i--;

		// checa se a ou b passou por alguma aresta mais pesada 
		// do que a que está salva em ret
        ret = max(ret, up[a][i].ss); 
        ret = max(ret, up[b][i].ss);

		// faz a ser igual ao (2^i)-ésimo nó acima dele
        a = up[a][i].ff;
		// faz b ser igual ao (2^i)-ésimo nó acima dele
        b = up[b][i].ff;
    }

	return ret;

}

void solve(){
	
	int N, R;

	cin >> N >> R;

	// Representação da lista de arestas do grafo dado
	vector<struct Edge> graph(R);

	// Como as queries dão como entrada quais nós estão sendo ligados pela aresta
	// utilizei uma hash para conseguir acessar o peso dessa aresta de forma
	// rápida	
	map<ii,int> Roads;

	for(int i=0;i<R;++i){
		cin >> graph[i].u >> graph[i].v >> graph[i].w;
		Roads[{graph[i].u,graph[i].v}] = graph[i].w;
		Roads[{graph[i].v,graph[i].u}] = graph[i].w;
	}

	// Ordenando as arestas em ordem crescente de peso
	sort(all(graph),
	[](struct Edge a, struct Edge other){
		if(a.w != other.w)
			return a.w < other.w;
		if(a.u != other.u)
			return a.u < other.u;
		return a.v < other.v; 
	});

	// Calculando a MST com o algoritmo de Kruskall	

	DSU uf(N+1);

	int mst_cost = 0;

	mst = vector<vector<ii>>(N+1,vector<ii>());
	up = vector<vector<ii>>(N+1,vector<ii>(LOG+1));
	deepth = vi(N+1);

	for(auto &[u, v, w] : graph){
		if(uf.find_set(u) != uf.find_set(v)){
			mst[u].pb({v,w});
			mst[v].pb({u,w});
			uf.union_set(u,v);
			mst_cost += w;
		}
	}

	// Define o nó acima do "1" como 0, um nó inválido pra essa questão. Dessa forma
	// caso algum outro nó tente subir mais nós do que a árvore tem, irá sempre 
	// ser direcionado para o nó 0
	up[1][0] = {1,0};
	
	dfs(1, 0, 0, 0);

	// Processamento das queries

	int q;

	cin >> q;

	while(q--){

		int u, v;

		cin >> u >> v;

		// custo da aresta mais pesada entre os nós u e v
		int cost = get_max_weight(u,v);

		// Para responder as queries a lógica utilizada é:
		// dada a mst do grafo adiciona a rota que o rei impõe que seja utilizada
		// após isso a árvore terá mais de uma rota entre os nós u e v
		// para retirar esse "desperdício" de recurso de forma que os 2 nós continuem
		// conectados basta retirar a aresta mais cara entre estes nós que não seja
		// a aresta que foi adicionada pelo rei.
		cout << mst_cost - cost + Roads[{u,v}] << endl;

	}

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
