#include "printer.h"
#include "ttps.h"
#include "scc.h"
#include "loop_handler.h"
#include <unordered_map>
#include <bits/stdc++.h>
#include <fstream>

using namespace std;

bool loadgraph(vector<int>& nodes, vector<vector<pair<int,int>>>& adj, vector<vector<pair<int,int>>>& adj_rev, int& sink) {
    int n; 
    if (!(cin >> n)) {
        cerr << "Error: failed to read number of nodes" << endl;
        return false;
    }
    if (n <= 0) {
        cerr << "Error: number of nodes must be positive, got " << n << endl;
        return false;
    }
    
    sink = n-1;
    adj.clear();
    adj_rev.clear();
    adj.resize(n);
    adj_rev.resize(n);

    for(int i = 0; i < n; i++){
        int m;
        if (!(cin >> m)) {
            cerr << "Error: failed to read edge count for node " << (i+1) << endl;
            return false;
        }
        if (m < 0) {
            cerr << "Error: negative edge count " << m << " for node " << (i+1) << endl;
            return false;
        }
        
        int v, w;
        for(int e = 0; e < m; e++){
            if (!(cin >> v >> w)) {
                cerr << "Error: failed to read edge (v w) for node " << (i+1) << endl;
                return false;
            }
            if (v <= 0 || v > n) {
                cerr << "Error: destination node " << v << " out of range [1.." << n << "] for node " << (i+1) << endl;
                return false;
            }
            adj[i].push_back({v-1, w});
            adj_rev[v-1].push_back({i, w});
        }
        nodes.push_back(i);
    }
    return true;
}

int main(int argc, char* argv[]){

    int n;
    int source = 0;
    int sink;

    vector<int> nodes;
    vector<vector<pair<int,int>>> adj;
    vector<vector<pair<int,int>>> adj_rev;

    ifstream infile;
    if (argc > 1) {
        infile.open(argv[1]);
        if (!infile.is_open()) {
            cerr << "Error: cannot open input file '" << argv[1] << "'" << endl;
            return EXIT_FAILURE;
        }
        cin.rdbuf(infile.rdbuf());
    }

    if (!loadgraph(nodes, adj, adj_rev, sink)) {
        cerr << "Failed to load graph" << endl;
        return EXIT_FAILURE;
    }

    n = sink + 1;

    unordered_map<int, NodeInfo> loop_map;

    vector<vector<int>> scc_inner;
    vector<vector<pair<int,int>>> adj_cond;

    int m = n;

    clock_t prev = clock();

    int scc_count = strongly_connected_components(nodes, adj, scc_inner, adj_cond);

    Node* root = get_loop_decomposition(m, nodes, adj, adj_rev, loop_map, n);

    if (root) {
        // vector<dp_state> dp_result = dp_solver(root);
        // // from dp_result, find the element with the highest max_length
        // auto max_elem_it = max_element(dp_result.begin(), dp_result.end(),
        //                                 [](const dp_state& a, const dp_state& b) {
        //                                     return a.max_length < b.max_length;
        //                                 });
        // if (max_elem_it != dp_result.end()) {
        //     cout << max_elem_it->max_length << " "
        //          << 1.0*(clock()-prev)/CLOCKS_PER_SEC << endl;
        // }
        printTree(root);
        delete_tree(root);
    } else {
        std::exit(EXIT_FAILURE);
    }

    return 0;
}
