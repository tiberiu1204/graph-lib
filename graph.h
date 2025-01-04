#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_map>
#include <unordered_set>
#include <vector>

typedef std::vector<std::vector<std::pair<int, int>>> GraphContainer;

class DSU {
private:
  std::vector<int> parent, rank;

public:
  DSU(int n);

  int find(int x);

  void unite(int x, int y);
};

class Graph {
public:
  Graph() = default;
  Graph(GraphContainer &g) {
    this->container = g;
    this->get_edgelist();
  }

  void print_graph();

  bool are_ciclu();
  int comp_conexe();
  bool este_conex();
  bool este_bipartit();
  std::vector<int> sortare_topologica();
  Graph neorientat();
  Graph kruskal();
  const std::vector<std::tuple<int, int, int>> &lista_muchii() const {
    return this->edge_list;
  }

protected:
  GraphContainer container;
  std::vector<int> node_list;
  std::vector<std::tuple<int, int, int>> edge_list;

private:
  void get_edgelist();
  std::vector<std::pair<int, int>> &vecini(int node) { return container[node]; }
  bool dfs_are_ciclu(int node, std::vector<int> &colors);
  void dfs_comp_conexe(int node, std::vector<int> &visited,
                       std::vector<int> &order, bool do_order = true);
};

#endif
