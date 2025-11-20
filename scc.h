#ifndef SCC_H
#define SCC_H

#include <vector>
#include <unordered_set>
using namespace std;

extern vector<bool> visited;

void dfs(unordered_set<int>const& nodes, int v, vector<vector<pair<int,int>>> const& adj, vector<int> &output, int source = -1, int sink = -1);
int strongly_connected_components(vector<int>const& nodes, vector<vector<pair<int,int>>> const& adj,
                                  vector<vector<int>> &components, vector<vector<pair<int,int>>>& adj_cond, int source = -1, int sink = -1);

#endif
