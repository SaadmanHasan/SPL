#include "loop_handler.h"
#include "scc.h"
#include "printer.h"
#include <bits/stdc++.h>

void print(vector<vector<pair<int,int>>> adj){
    for(const auto& vec : adj) {
        for(const auto& p : vec) {
            cout << "(" << p.first << ", " << p.second << ") ";
        }
        cout << "\n";
    }
}

void print(vector<int> nodes){
    for(const auto& node : nodes) {
        cout << node << " ";
    }
    cout << "\n";
}

using namespace std;

Node* get_loop_decomposition(int& next_node, vector<int> nodes, vector<vector<pair<int,int>>>& adj, vector<vector<pair<int,int>>>& adj_rev, unordered_map<int, NodeInfo>& loop_map, int num){

    int source = *min_element(nodes.begin(), nodes.end());
    int sink = *max_element(nodes.begin(), nodes.end());

    vector<vector<int>> scc_inner;
    vector<vector<pair<int,int>>> adj_cond;
    int scc_count = strongly_connected_components(nodes, adj, scc_inner, adj_cond);
    
    int n = nodes.size();

    // No loops, find decomposition tree with ttps
    if(scc_count == n){
        // Find the decomposition tree using the TTSP algorithm
        Node* root = get_decomposition_tree(num, source, sink, nodes, adj, loop_map);
        return root;
    }

    //There are inner loops, find the decomposition tree with recursion 
    else{
        for(auto& component: scc_inner){

            if(component.size() == 1) continue;

            int inner_source = *min_element(component.begin(), component.end());
            int inner_sink = *max_element(component.begin(), component.end());

            int do_while = 0;
            auto max_pair_inner = *max_element(adj[inner_source].begin(), adj[inner_source].end(), 
                                                [](const pair<int,int>& a, const pair<int,int>& b) {
                                                    return a.first < b.first;
                                                });
            if(max_pair_inner.first <= inner_sink){
                do_while = 1;
            }

            if((component.size() == 2 && adj[inner_source].size() == 1 && do_while) || 
               (component.size() == 2 && adj[inner_source].size() == 2 && !do_while)){
                
                int act_weight = 0, break_weight = 0, cont_weight = 0;
                int st = 0, t1s = 0, t1t = 0, ss1 = 0;
                for(const auto& [v,w]: adj[inner_sink]){
                    if (v == inner_source){
                        t1s = w;
                        break;
                    }
                }
                for(const auto& [v,w]: adj[inner_source]){
                    if (v == inner_sink){
                        act_weight = w;
                        break;
                    }
                }
                

                st = max_pair_inner.second;

                if(do_while){
                    // Do-While Loop
                    do_while = 1;
                    auto temp = *max_element(adj[inner_sink].begin(), adj[inner_sink].end(),
                                              [](const pair<int,int>& a, const pair<int,int>& b) {
                                                  return a.first < b.first;
                                              });
                    max_pair_inner = temp;
                    t1t = max_pair_inner.second;

                    int s = next_node++;
                    int b = next_node++;
                    int c = next_node++;
                    int b1 = next_node++;
                    int c1 = next_node++;
                    int t = max_pair_inner.first;
                    
                    loop_edge_info lei = loop_edge_info(-1, 0, t1s, t1t, 0, 0);
                    
                    Node* leaf = new Node(inner_source, inner_sink, act_weight);

                    loop_node_info lni = loop_node_info(s, t, b, c, inner_sink, b1, c1, inner_source);
                    loop_map[inner_source] = NodeInfo(DO_WHILE, act_weight, leaf, lei, lni);
                }
                else{
                    int s = next_node++;
                    int t = max_pair_inner.first;
                    int b = next_node++;
                    int c = next_node++;
                    int b1 = next_node++;
                    int c1 = next_node++;

                    Node* leaf = new Node(inner_source, inner_sink, act_weight);
                    loop_edge_info lei = loop_edge_info(st, 0, t1s, -1, 0, 0);
                    loop_node_info lni = loop_node_info(s, t, b, c, inner_sink, b1, c1, inner_source);

                    loop_map[inner_source] = NodeInfo(WHILE, act_weight, leaf, lei, lni);
                }

            }

            else{

                // Find Continue and Break nodes, bla bla etc. 
                int do_while = 0;

                //First, fix the continue edges
                int found_continue = 0, found_break = 0;
                int c, b;
                int c1 = 0, b1 = 0, s1 = 0, t1 = inner_sink; 
                int s = inner_source;
                int cont_weight = 0, break_weight = 0;
                int st = 0, ss1 = 0, t1s = 0, t1t = 0; 

                for(const auto& [v,w]: adj_rev[inner_source]){
                    if(v > inner_source && v < inner_sink){
                        if(found_continue == 0){
                            found_continue = 1;
                            c1 = next_node++;
                        }
                        // cont_weight += w;
                        adj[v].clear();
                        adj[v].push_back({inner_sink, w});
                        loop_map[v] = NodeInfo(CONTINUE, v, c1, 0);
                    }
                }


                //Check if while or do-while loop 
                auto [t, ts] = *max_element(adj[inner_source].begin(), adj[inner_source].end(), 
                                            [](const pair<int,int>& a, const pair<int,int>& b) {
                                                return a.first < b.first;
                                            });

                st = ts;

                if(t <= inner_sink){
                    //DO-while Loop
                    do_while = 1;
                    auto max_pair_sink = *max_element(adj[inner_sink].begin(), adj[inner_sink].end(), 
                                                      [](const pair<int,int>& a, const pair<int,int>& b) {
                                                          return a.first < b.first;
                                                      });
                    t = max_pair_sink.first;
                    t1t = max_pair_sink.second;
                }
                
                // Now, fix the break edges
                unordered_map<int,bool> vst;
                for(const auto& node: component){
                    vst[node] = true;
                }
                queue<int> q;
                for(auto& [v,w]: adj_rev[t]){
                    if(v > inner_source && v < inner_sink){
                        if(found_break == 0){
                            found_break = 1;
                            b1 = next_node++;
                        }
                        // break_weight += w;
                        adj[v].clear();
                        adj[v].push_back({inner_sink, w});
                        q.push(v);
                        vst[v] = true;
                        while(!q.empty()){
                            int curr = q.front();
                            q.pop();
                            component.push_back(curr);
                            for(const auto& [nxt, w]: adj_rev[curr]){
                                if(nxt != inner_source && !vst[nxt]){
                                    q.push(nxt);
                                    vst[nxt] = true;
                                }
                            }
                        }

                        // cout << v << " " << b1 << "\n";
                        loop_map[v] = NodeInfo(BREAK, v, b1, 0); 
                    }
                }

                t1s = 0;
                for(const auto& [v,w]: adj[inner_sink]){
                    if(v == inner_source){
                        t1s = w;
                        break; 
                    }
                }
                if(do_while){
                    if(adj[inner_source].size() == 1){
                        if(b1 == 0){
                            b1 = next_node++;
                        }
                        if(c1 == 0){
                            c1 = next_node++;
                        }
                        s1 = adj[inner_sink][0].first;
                        c = next_node++;
                        b = next_node++;
                        ss1 = adj[inner_source][0].second;
                        component.erase(std::remove(component.begin(), component.end(), inner_source), component.end());
                    }
                    else{
                        adj[inner_sink].erase(
                            std::remove_if(
                                adj[inner_sink].begin(),
                                adj[inner_sink].end(),
                                [inner_source, t1s](const pair<int, int>& p) {
                                    return p.first == inner_source && p.second == t1s;
                                }
                            ),
                            adj[inner_sink].end()
                        );
                        if(b1 == 0){
                            b1 = next_node++;
                        }
                        if(c1 == 0){
                            c1 = next_node++;
                        }
                        s1 = inner_source;
                        s = next_node++;
                        c = next_node++;
                        b = next_node++;
                        ss1 = 0;

                    }
                }
                else{
                    if(adj[inner_source].size() == 2){
                        for(const auto& [v,w]: adj[inner_source]){
                            if(v != t){
                                s1 = v;
                                ss1 = w;
                                component.erase(std::remove(component.begin(), component.end(), inner_source), component.end());
                            }
                            else{
                                st = w;
                            }
                        }
                        if(b1 == 0){
                            b1 = next_node++;
                        }
                        if(c1 == 0){
                            c1 = next_node++;
                        }
                        b = next_node++;
                        c = next_node++;
                        // cout << "s1 is: " << s1 << "\n";

                    }
                    else{ 
                        adj[inner_sink].erase(
                            std::remove_if(
                                adj[inner_sink].begin(),
                                adj[inner_sink].end(),
                                [inner_source, t1s](const pair<int, int>& p) {
                                    return p.first == inner_source && p.second == t1s;
                                }
                            ),
                            adj[inner_sink].end()
                        );
                        s1 = inner_source;
                        ss1 = 0;
                        s = next_node++;
                        c = next_node++;
                        b = next_node++;
                        if(b1 == 0){
                            b1 = next_node++;
                        }
                        if(c1 == 0){
                            c1 = next_node++;
                        }
                    }
                    

                }

                // print(component);

                Node* child = get_loop_decomposition(next_node, component, adj, adj_rev, loop_map, num);

                if(child == nullptr){
                    std::cerr << "Graph is not SPL Decomposable" << std::endl;
                    std::exit(EXIT_FAILURE);
                }

                adj[inner_sink].push_back({inner_source, t1s});

                loop_edge_info lei = loop_edge_info(st, ss1, t1s, t1t, break_weight, cont_weight);
                loop_node_info lni = loop_node_info(s,t,b,c,t1,b1,c1,s1);

                loop_map[inner_source] = do_while? NodeInfo(DO_WHILE, t1t, child, lei, lni): NodeInfo(WHILE, st, child, lei, lni);

            }
        }

        strongly_connected_components(nodes, adj, scc_inner, adj_cond);

        Node* root = get_decomposition_tree(num, source, sink, nodes, adj_cond, loop_map);

        return root;
    }
    
    return nullptr; // Add missing return statement
}
