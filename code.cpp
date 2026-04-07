#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>

using namespace std;

const int N = 100005;
vector<int> adj[N];
int dfn[N], low[N], timer;
stack<pair<int, int>> st;
vector<vector<pair<int, int>>> bccs;

void dfs(int u, int p = 0) {
    dfn[u] = low[u] = ++timer;
    for (int v : adj[u]) {
        if (v == p) continue;
        if (dfn[v]) {
            low[u] = min(low[u], dfn[v]);
            if (dfn[v] < dfn[u]) {
                st.push({u, v});
            }
        } else {
            st.push({u, v});
            dfs(v, u);
            low[u] = min(low[u], low[v]);
            if (low[v] >= dfn[u]) {
                vector<pair<int, int>> bcc_edges;
                while (true) {
                    auto edge = st.top();
                    st.pop();
                    bcc_edges.push_back(edge);
                    if (edge == make_pair(u, v)) break;
                }
                bccs.push_back(bcc_edges);
            }
        }
    }
}

vector<int> bcc_adj[N];
int color[N];
bool good[N];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    if (!(cin >> n >> m)) return 0;
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    for (int i = 1; i <= n; ++i) {
        if (!dfn[i]) {
            dfs(i);
        }
    }

    for (const auto& bcc_edges : bccs) {
        vector<int> bcc_nodes;
        for (auto edge : bcc_edges) {
            int u = edge.first;
            int v = edge.second;
            bcc_adj[u].push_back(v);
            bcc_adj[v].push_back(u);
            bcc_nodes.push_back(u);
            bcc_nodes.push_back(v);
        }
        
        sort(bcc_nodes.begin(), bcc_nodes.end());
        bcc_nodes.erase(unique(bcc_nodes.begin(), bcc_nodes.end()), bcc_nodes.end());

        bool is_bipartite = true;
        queue<int> q;
        int start = bcc_nodes[0];
        color[start] = 1;
        q.push(start);

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : bcc_adj[u]) {
                if (color[v] == 0) {
                    color[v] = 3 - color[u];
                    q.push(v);
                } else if (color[v] == color[u]) {
                    is_bipartite = false;
                }
            }
        }

        if (!is_bipartite) {
            for (int u : bcc_nodes) {
                good[u] = true;
            }
        }

        for (int u : bcc_nodes) {
            color[u] = 0;
            bcc_adj[u].clear();
        }
    }

    int ans = 0;
    for (int i = 1; i <= n; ++i) {
        if (!good[i]) {
            ans++;
        }
    }

    cout << ans << "\n";
    return 0;
}
