#include <bits/stdc++.h>
#include "ttps.h"
#include "loop_handler.h"
using namespace std;

// As input will require the original CFG and the SPL decomposition tree
void reconstructCFG(Node* root, map<int, map<int, int>>&CFG) {
    if (!root) return;

    switch (root->type) {
        case NodeType::LEAF:
            CFG[root->u][root->v] = root->weight;
            break;
        case NodeType::BREAK:
        case NodeType::CONTINUE:
            CFG[root->brk_or_cont_start][root->brk_or_cont_end] = root->weight;
            break;
        case NodeType::SERIES:
        case NodeType::PARALLEL:
            for (int i = 0; i < root->children.size(); i++) reconstructCFG(root->children[i], CFG);
            break;

        case NodeType::WHILE:
        case NodeType::DO_WHILE:
            // Process the children first
            for (Node* child : root->children) reconstructCFG(child, CFG);
            // Then add the loop edges
            CFG[root->lni.s][root->lni.s1] = root->lei.ss1;
            CFG[root->lni.t1][root->lni.s] = root->lei.t1s;
            CFG[root->lni.b1][root->lni.t] = root->lei.b1t;
            CFG[root->lni.c1][root->lni.s] = root->lei.c1s;
            
            if (root->type == NodeType::WHILE) CFG[root->lni.s][root->lni.t] = root->lei.st;
            if (root->type == NodeType::DO_WHILE) CFG[root->lni.t1][root->lni.t] = root->lei.t1t;
            break;
    }
}
