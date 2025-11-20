#ifndef TTPS_H
#define TTPS_H

#include <vector>
#include <queue>
#include <list>
#include <set>
#include <map>
#include <iostream>
#include <algorithm>
#include <unordered_map>

using namespace std;

enum NodeType { LEAF, SERIES, PARALLEL, WHILE, DO_WHILE, BREAK, CONTINUE};

struct loop_node_info{
    int s;
    int t; 
    int b; 
    int c; 
    int t1;
    int b1; 
    int c1; 
    int s1; 
    loop_node_info():s(0), t(0), b(0), c(0), t1(0), b1(0), c1(0), s1(0){}
    loop_node_info(int s, int t, int b, int c, int t1, int b1, int c1, int s1) 
        : s(s), t(t), b(b), c(c), t1(t1), b1(b1), c1(c1), s1(s1) {}
};  

struct loop_edge_info{
    int st;
    int ss1;
    int t1s;
    int t1t;
    int b1t;
    int c1s;
    loop_edge_info():st(0), ss1(0), t1s(0), t1t(0), b1t(0), c1s(0){}
    loop_edge_info(int st, int ss1, int t1s, int t1t, int b1t, int c1s) : st(st), ss1(ss1), t1s(t1s), t1t(t1t), b1t(b1t), c1s(c1s) {}

};

struct Node {
    int u, v; 
    int weight = 0;
    NodeType type;          // Only for LEAF, CONT and BREAK nodes
    vector<Node*> children;
    loop_edge_info lei;
    loop_node_info lni;
    int brk_or_cont_start;
    int brk_or_cont_end;
    Node(NodeType t) : type(t), lei(loop_edge_info()), lni(loop_node_info()) {}
    Node(int u0, int v0, int w) : type(LEAF), u(u0), v(v0), weight(w) {}
    Node(int uo, int v0, int w, NodeType t) : type(t), brk_or_cont_start(uo), brk_or_cont_end(v0), weight(w), lei(loop_edge_info()), lni(loop_node_info()) {} // Other than Series and Parallel nodes 
    Node(int uo, int v0, int w, NodeType t, loop_edge_info lei, loop_node_info lni) : type(t), u(uo), v(v0), weight(w), lei(lei), lni(lni) {}

};


struct NodeInfo {
    NodeType type;
    int weight = -1; 
    Node* node = nullptr;
    loop_edge_info lei;
    loop_node_info lni; 
    int brk_or_cont_start;
    int brk_or_cont_end;
    // Default constructor
    NodeInfo() : type(LEAF), weight(0), node(nullptr), lei(loop_edge_info()), lni(loop_node_info()) {}

    // Parameterized constructor
    NodeInfo(NodeType t, int bcs, int bce, int w): type(t), weight(w), brk_or_cont_start(bcs), brk_or_cont_end(bce) {}
    NodeInfo(NodeType t, int w, Node* node, loop_edge_info lei, loop_node_info lni): type(t), weight(w), node(node), lei(lei), lni(lni) {}
};


void delete_tree(Node* root);
Node* get_decomposition_tree(int n, int source, int sink, vector<int> nodes, 
                            vector<vector<pair<int,int>>>const & adj, unordered_map<int, NodeInfo> &loop_map);

#endif
