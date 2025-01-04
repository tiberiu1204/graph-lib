#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_map>
#include <unordered_set>
#include <vector>

typedef std::unordered_map<
    int,
    std::vector<std::pair<int, int>>> // node, vector of (neighbour, weight)
    GraphContainer;

class Graph {
public:
  Graph() = default;
  Graph(GraphContainer &g) { this->container = g; }

  void print_graph();
  void delete_node(int node);
  void delete_edge(int node1, int node2, bool directed = false);

  bool are_ciclu();
  int comp_conexe();
  bool este_conex();
  std::vector<int> sortare_topologica();

protected:
  GraphContainer container;

private:
  std::vector<std::pair<int, int>> &vecini(int node) { return container[node]; }
  bool dfs_are_ciclu(int node, std::unordered_map<int, int> &colors);
  void dfs_comp_conexe(int node, std::unordered_set<int> &visited,
                       std::vector<int> &order, bool do_order = true);
};

#endif
