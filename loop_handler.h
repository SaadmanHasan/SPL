#ifndef LOOP_HANDLER_H
#define LOOP_HANDLER_H

#include "ttps.h"
#include <vector>
#include <unordered_map>
using namespace std;

Node* get_loop_decomposition(int& next_node, vector<int> nodes, vector<vector<pair<int,int>>>& adj, vector<vector<pair<int,int>>>& adj_rev, unordered_map<int, NodeInfo>& loop_map, int num);

void print(vector<vector<pair<int,int>>> adj);
void print(vector<int> nodes);

void reconstructCFG(Node* root, map<int, map<int, int>>&CFG);

#endif

