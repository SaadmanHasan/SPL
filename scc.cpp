#include "scc.h"
#include <bits/stdc++.h>
using namespace std;

vector<bool> visited;

void dfs(unordered_set<int>const& nodes, int v, vector<vector<pair<int,int>>> const& adj, vector<int> &output, int source, int sink) {
    
    if (v == source || v == sink || nodes.find(v) == nodes.end()) return;
    
    visited[v] = true;
    for (auto [u, w] : adj[v])
        if (u != source && u != sink && !visited[u] && nodes.find(u) != nodes.end())
            dfs(nodes, u, adj, output, source, sink);
    output.push_back(v);
}

int strongly_connected_components(vector<int>const& nodeset, vector<vector<pair<int,int>>> const& adj,
                                  vector<vector<int>> &components, vector<vector<pair<int,int>>>& adj_cond, int source, int sink) {

    // Use the actual size of adj, not just node count
    int n = adj.size();
    
    components.clear();
    adj_cond.clear();

    vector<int> order; 

    visited.assign(n, false);
    
    unordered_set<int> nodes(nodeset.begin(), nodeset.end());

    for(auto v: nodes){
        if (v == source || v == sink || nodes.find(v) == nodes.end()) continue;
        if (!visited[v]) {
            dfs(nodes, v, adj, order, source, sink);
        }
    }


    
    // for (int i = 0; i < n; i++)
    //     if (!visited[i])
    //         dfs(i, adj, order);

    vector<vector<pair<int,int>>> adj_rev(n);
    for (int v = 0; v < n; v++)
        for (auto [u, w] : adj[v])
            adj_rev[u].push_back({v,w});

    visited.assign(n, false);
    reverse(order.begin(), order.end());

    vector<int> roots(n, 0);

    int num_components = 0;

    for (auto v : order){
        if (!visited[v]) {
            std::vector<int> component;
            dfs(nodes, v, adj_rev, component, source, sink);
            components.push_back(component);
            int root = *min_element(begin(component), end(component));
            for (auto u : component) {
                roots[u] = root;
            }
            num_components++;

        }
    }

    adj_cond.assign(n, {});
    for (auto v : nodes) {
        if (v == source || v == sink || nodes.find(v) == nodes.end()) continue;
        for (auto [u, w] : adj[v]){
            if (nodes.find(u) == nodes.end()) continue;
            if (roots[v] != roots[u])
                adj_cond[roots[v]].push_back({roots[u],w});
        }
    }
    return num_components;
}
