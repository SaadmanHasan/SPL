#include <bits/stdc++.h>
#include <stack>
using namespace std;

bool isSPL(int cntNode, vector<vector<int>> &CFGIn, vector<vector<int>> &CFGOut, unordered_set<int> &nodes, int entryNode) {
    if (nodes.size() < 2) return true;
    int cntInEdges = 0;
    for (int node: nodes) {
        for (int pred: CFGIn[node]) {
            if (nodes.count(pred) == 0) {
                cntInEdges++;
                break;  // Only count once per node
            }
        }
    }

    if (cntInEdges > 1) return false;
    
    int cntOutEdges = 0;
    for (int node: nodes) {
        for (int succ: CFGOut[node]) {
            if (nodes.count(succ) == 0) {
                cntOutEdges++;
                break;  // Only count once per node
            }
        }
    }
    if (cntOutEdges > 1) return false;

    // Need to remove the backedge from the provided nodes
    // Start with entry node and remove the backedge
    bool visited[cntNode + 1] = {false};
    stack<int> queue;
    queue.push(entryNode);
    while (!queue.empty()) {
        int u = queue.top();
        queue.pop();
        if (visited[u]) continue;
        visited[u] = true;

        for (int v: CFGOut[u]) {
            if (v == entryNode) {
                // Remove the backedge to entry node
                CFGOut[u].erase(remove(CFGOut[u].begin(), CFGOut[u].end(), v), CFGOut[u].end());
                CFGIn[v].erase(remove(CFGIn[v].begin(), CFGIn[v].end(), u), CFGIn[v].end());
            } else if (nodes.count(v) && !visited[v]) {
                queue.push(v);
            }
            if (nodes.count(v) && !visited[v]) {
                queue.push(v);
            }
        }
    }
    
    // Find the strongly connected components (SCCs)
    vector<int> low(cntNode + 1, 0), disc(cntNode + 1, 0);
    vector<bool> inStack(cntNode + 1, false);
    stack<int> st;
    vector<vector<int>> components;
    int time = 0;
    function<void(int)> tarjan = [&](int u) {
        low[u] = disc[u] = ++time;
        st.push(u);
        inStack[u] = true;

        for (int v : CFGOut[u]) {
            if (disc[v] == 0) {
                tarjan(v);
                low[u] = min(low[u], low[v]);
            } else if (inStack[v]) {
                low[u] = min(low[u], disc[v]);
            }
        }

        if (low[u] == disc[u]) {
            vector<int> component;
            while (st.top() != u) {
                int v = st.top();
                st.pop();
                inStack[v] = false;
                component.push_back(v);
            }
            st.pop();
            inStack[u] = false;
            component.push_back(u);
            components.push_back(component);
        }
    };

    for (int i = 1; i <= cntNode; i++) {
        if (disc[i] == 0) {
            tarjan(i);
        }
    }


    // Check if all scc are SPL decomposable
    for (const auto &component : components) {
        entryNode = component[0];
        unordered_set<int> componentSet(component.begin(), component.end());
        if (!isSPL(cntNode, CFGIn, CFGOut, componentSet, entryNode)) {
            return false;
        }
    }
    return true;
}

// int main() {
//     int cntNode;
//     cin >> cntNode;
//     vector<vector<int>> CFGOut(cntNode + 1, vector<int>());
//     vector<vector<int>> CFGIn(cntNode + 1, vector<int>());

//     vector<bool> entryNode(cntNode + 1, true);

//     for (int u = 1; u <= cntNode; u++) {
//         int cntSucc;
//         cin >> cntSucc;
//         for (int i = 0; i < cntSucc; i++) {
//             int v, w;
//             cin >> v >> w;
//             CFGOut[u].push_back(v);
//             CFGIn[v].push_back(u);
//             entryNode[v] = false;
//         }
//     }
//     // Nodes are all the nodes from 1 to cntNode initially
//     unordered_set<int> nodes;
//     for (int i = 1; i <= cntNode; i++) {
//         nodes.insert(i);
//     }
    
//     for (int entry: entryNode) {
//         if (!isSPL(cntNode, CFGIn, CFGOut, nodes, entry)) {
//             cout << "The graph is not SPL." << endl;
//             return 0;
//         }
//     }
//     cout << "The graph is SPL." << endl;
//     return 0;
// }
