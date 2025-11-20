#include "printer.h"
#include <bits/stdc++.h>
using namespace std;

void printTree(Node* root, const string& prefix, bool isLast) {
    if (root == nullptr) return;

    cout << prefix;
    cout << (isLast ? "+-- " : "+-- ");

    switch (root->type) {
        case LEAF:
            cout << "LEAF: (" << root->u << ", " << root->v << ", " << root->weight << ")" << endl;
            break;
        case SERIES:
            cout << "SERIES" << endl;
            break;
        case PARALLEL:
            cout << "PARALLEL" << endl;
            break;
        case WHILE:
            cout << "WHILE: (" << root->lni.s << ", " << root->lni.s1 << ", " << root->lni.t1 << ", " 
            << root->lni.t << ", " << root->lni.c << ", " 
            << root->lni.b << ", " << root->lni.c1 << ", " << root->lni.b1 << ", " << root->weight << "), (" 
            << root->lei.st << ", " << root->lei.ss1 << ", " << root->lei.c1s << ", " << root->lei.b1t << 
            ", " << root->lei.t1s << ")" << endl;
            break;
        case DO_WHILE:
            cout << "DO_WHILE: (" << root->lni.s << ", " << root->lni.s1 << ", " << root->lni.t1 <<
            ", " << root->lni.t << ", " << root->lni.c << ", " 
            << root->lni.b << ", " << root->lni.c1 << ", " << root->lni.b1 << ", " << root->weight << "), (" 
            << root->lei.t1t << ", " << root->lei.ss1 << ", " << root->lei.c1s << ", " << root->lei.b1t << 
            ", " << root->lei.t1s << ")" << endl;
            break;
        case BREAK:
            cout << "BREAK: (" << root->brk_or_cont_start << ", " << root->brk_or_cont_end << ", " 
            << root->weight << ")" << endl;
            break;
        case CONTINUE:
            cout << "CONTINUE: (" << root->brk_or_cont_start << ", " << root->brk_or_cont_end << ", " 
            << root->weight << ")" << endl;
            break;
    }

    string newPrefix = prefix + (isLast ? "    " : "|   ");
    for (size_t i = 0; i < root->children.size(); ++i) {
        bool lastChild = (i == root->children.size() - 1);
        printTree(root->children[i], newPrefix, lastChild);
    }
}

// int main(){
//     int n = 4;
//     int source = 0;
//     int sink = 3;
//     vector<int> nodes = {0, 1, 2, 3};
//     vector<vector<int>> adj = {{1,2},{3},{3},{}};

//     unordered_map<int, Node*> loop_map;

//     Node* root = get_decomposition_tree(n, source, sink, nodes, adj, loop_map);
//     if (root) {
//         cout << "Decomposition tree root obtained." << endl;
//         printTree(root);
//         delete_tree(root);
//     } else {
//         cout << "Graph is not TTSP." << endl;
//     }
//     return 0;
// }
