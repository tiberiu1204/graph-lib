#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Edge {
  int dest;
  int weight = 0;
  int flow = 0;
  int cap = 0;
};

typedef std::vector<std::vector<Edge>> GraphContainer;

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
    this->container = std::move(g);
    this->get_edgelist();
  }
  Graph(GraphContainer &&g) {
    this->container = std::move(g);
    this->get_edgelist();
  }

  void add_edge(int node1, int node2, int weight, int flow, int cap);
  void remove_edge(int node1, int node2);
  void add_node();
  size_t size() const;
  std::vector<Edge> &edges(int node) { return container[node]; }

  void print_graph();
  int greutate();
  Graph neorientat();

  bool are_ciclu();
  int comp_conexe();
  bool este_conex();
  std::unordered_map<int, int> este_bipartit();
  std::vector<int> sortare_topologica();
  Graph kruskal(std::pair<int, int> force_take = {-1, -1});
  std::vector<int> puncte_critice();
  std::vector<int> dijkstra(int src_node);
  int edmonds_karp(int s, int t);
  int cuplu_max();
  std::pair<std::vector<int>, std::vector<int>> bellman_ford(int src);
  const std::vector<std::tuple<int, int, int>> &lista_muchii() const {
    return this->edge_list;
  }

protected:
  GraphContainer container;
  std::vector<int> node_list;
  std::vector<std::tuple<int, int, int>> edge_list;

private:
  void get_edgelist();
  bool dfs_are_ciclu(int node, std::vector<int> &colors);
  void dfs_comp_conexe(int node, std::vector<int> &visited,
                       std::vector<int> &order, bool do_order = true);
  void dfs_puncte_critice(int node, int parent, std::vector<bool> &visited,
                          std::vector<int> &disc, std::vector<int> &low,
                          int &time, std::vector<bool> &critical);
};

#endif
