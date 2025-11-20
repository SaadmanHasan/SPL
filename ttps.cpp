#include "ttps.h"
#include "printer.h"

void delete_tree(Node* root) {
    if (root) {
        for (Node* child : root->children) {
            delete_tree(child);
        }
        delete root;
    }
}

Node* get_decomposition_tree(int n, int source, int sink, vector<int> nodes, 
                            vector<vector<pair<int,int>>>const& adj, unordered_map<int, NodeInfo> &loop_map) {
    vector<int> in_deg(n, 0);
    vector<int> out_deg(n, 0);
    vector<set<int>> in_neighbors(n);
    vector<set<int>> out_neighbors(n);
    map<pair<int, int>, list<Node*>> edge_map;

    for(const auto& v: nodes){
        for(auto [u,w]: adj[v]){
            // Should add another checker if u belongs to nodes 
            if(u > sink || u < source) continue; 
            // Check for Do-while, While, Break and Continue nodes  
            if(loop_map.find(v) != loop_map.end()){

                switch(loop_map[v].type){
                    case WHILE: {
                        Node* loop_node = new Node(v, u, w, WHILE, loop_map[v].lei, loop_map[v].lni);
                        loop_node->children.push_back(loop_map[v].node);
                        edge_map[{v, u}].push_back(loop_node);
                        break;
                    }
                    
                    case DO_WHILE: {
                        Node* do_while_node = new Node(v, u, w, DO_WHILE, loop_map[v].lei, loop_map[v].lni);
                        do_while_node->children.push_back(loop_map[v].node);
                        edge_map[{v, u}].push_back(do_while_node);
                        break;
                    }

                    case BREAK: {
                        Node* break_node = new Node(loop_map[v].brk_or_cont_start, loop_map[v].brk_or_cont_end, w, BREAK);
                        edge_map[{v, u}].push_back(break_node);
                        break;
                    }

                    case CONTINUE: {
                        Node* continue_node = new Node(loop_map[v].brk_or_cont_start, loop_map[v].brk_or_cont_end, w, CONTINUE);
                        edge_map[{v, u}].push_back(continue_node);
                        break;
                    }
                }
            }
            else{
                if(loop_map.find(u) != loop_map.end()){
                    if(loop_map[u].type ==  DO_WHILE || loop_map[u].type == WHILE){
                        if(loop_map[u].lni.s >= n){
                            Node* leaf = new Node(v, loop_map[u].lni.s, w);
                            edge_map[{v, u}].push_back(leaf);
                        }
                        else{
                            Node* leaf = new Node(v,u,w);
                            edge_map[{v, u}].push_back(leaf);
                        }
                    }
                    else{
                        Node* leaf = new Node(v,u,w);
                        edge_map[{v, u}].push_back(leaf);
                    }
                }
                else{
                    Node* leaf = new Node(v,u,w);
                    edge_map[{v, u}].push_back(leaf);
                }
            }
            in_deg[u]++;
            out_deg[v]++;

            if (in_neighbors[u].find(v) == in_neighbors[u].end()) {
                in_neighbors[u].insert(v);
            }
            if (out_neighbors[v].find(u) == out_neighbors[v].end()) {
                out_neighbors[v].insert(u);
            }
        }
    }

    queue<int> Q_series;
    queue<pair<int, int>> Q_parallel;

    for (int i = 0; i < n; i++) {
        if (i == source || i == sink) continue;
        if (in_deg[i] == 1 && out_deg[i] == 1) {
            Q_series.push(i);
        }
    }  

    for (const auto& entry : edge_map) {
        if (entry.second.size() >= 2) {
            Q_parallel.push(entry.first);
        }
    }

    while (!Q_series.empty() || !Q_parallel.empty()) {
        if (!Q_parallel.empty()) {
            pair<int, int> uv = Q_parallel.front();
            Q_parallel.pop();
            int u = uv.first;
            int v = uv.second;

            if (edge_map.find(uv) == edge_map.end() || edge_map[uv].size() < 2) {
                continue;
            }

            list<Node*> L = move(edge_map[uv]);
            edge_map.erase(uv);

            in_deg[v] -= L.size();
            out_deg[u] -= L.size();

            in_neighbors[v].erase(u);
            out_neighbors[u].erase(v);

            Node* P = new Node(PARALLEL);
            P->children = vector<Node*>(L.begin(), L.end());

            pair<int, int> new_edge = {u, v};
            edge_map[new_edge].push_back(P);

            in_deg[v]++;
            out_deg[u]++;

            if (in_neighbors[v].find(u) == in_neighbors[v].end()) {
                in_neighbors[v].insert(u);
            }
            if (out_neighbors[u].find(v) == out_neighbors[u].end()) {
                out_neighbors[u].insert(v);
            }

            if (u != source && u != sink && in_deg[u] == 1 && out_deg[u] == 1) {
                Q_series.push(u);
            }
            if (v != source && v != sink && in_deg[v] == 1 && out_deg[v] == 1) {
                Q_series.push(v);
            }
        }
        else if (!Q_series.empty()) {
            int v = Q_series.front();
            Q_series.pop();

            if (v == source || v == sink || in_deg[v] != 1 || out_deg[v] != 1) {
                continue;
            }
            if (in_neighbors[v].size() != 1 || out_neighbors[v].size() != 1) {
                continue;
            }

            int u = *(in_neighbors[v].begin());
            int w = *(out_neighbors[v].begin());

            pair<int, int> uv_edge = {u, v};
            if (edge_map.find(uv_edge) == edge_map.end() || edge_map[uv_edge].size() != 1) {
                continue;
            }
            Node* uv_node = edge_map[uv_edge].front();
            edge_map.erase(uv_edge);

            pair<int, int> vw_edge = {v, w};
            if (edge_map.find(vw_edge) == edge_map.end() || edge_map[vw_edge].size() != 1) {
                continue;
            }
            Node* vw_node = edge_map[vw_edge].front();
            edge_map.erase(vw_edge);

            in_deg[v]--;
            out_deg[u]--;
            out_deg[v]--;
            in_deg[w]--;

            in_neighbors[v].erase(u);
            out_neighbors[u].erase(v);
            out_neighbors[v].erase(w);
            in_neighbors[w].erase(v);

            Node* S = new Node(SERIES);
            S->children.push_back(uv_node);
            S->children.push_back(vw_node);

            pair<int, int> new_edge = {u, w};
            edge_map[new_edge].push_back(S);

            out_deg[u]++;
            in_deg[w]++;

            if (in_neighbors[w].find(u) == in_neighbors[w].end()) {
                in_neighbors[w].insert(u);
            }
            if (out_neighbors[u].find(w) == out_neighbors[u].end()) {
                out_neighbors[u].insert(w);
            }

            if (u != source && u != sink && in_deg[u] == 1 && out_deg[u] == 1) {
                Q_series.push(u);
            }
            if (w != source && w != sink && in_deg[w] == 1 && out_deg[w] == 1) {
                Q_series.push(w);
            }

            if (edge_map[new_edge].size() >= 2) {
                Q_parallel.push(new_edge);
            }
        }
    }
    

    if (edge_map.size() == 1) {
        auto it = edge_map.begin();
        if (it->first == make_pair(source, sink) && it->second.size() == 1) {
            return it->second.front();
        }
    }

    for (auto& entry : edge_map) {
        for (Node* node : entry.second) {
            delete_tree(node);
        }
    }
    return nullptr;
}



