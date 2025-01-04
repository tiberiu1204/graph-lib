#include "graph.h"
#include <iostream>

void Graph::print_graph() {
  for (const auto &kvpair : container) {
    int node = kvpair.first;
    std::cout << "Node " << node << ": ";
    for (const auto &neighbor : kvpair.second) {
      std::cout << "(" << neighbor.first << ", " << neighbor.second << ") ";
    }
    std::cout << std::endl;
  }
}

void Graph::delete_node(int node) {
  this->container.erase(node);
  for (auto &kvpair : this->container) {
    for (auto it = kvpair.second.begin(); it < kvpair.second.end(); it++) {
      if (it->first == node) {
        this->container[kvpair.first].erase(it);
        break;
      }
    }
  }
}

void Graph::delete_edge(int node1, int node2, bool directed) {
  for (auto it = this->container[node1].begin();
       it < this->container[node1].end(); it++) {
    if (it->first == node2) {
      this->container[node1].erase(it);
      break;
    }
  }
  if (!directed) {
    this->delete_edge(node2, node1, true);
  }
}

bool Graph::dfs_are_ciclu(int node, std::unordered_map<int, int> &colors) {
  colors[node] = 2;
  std::vector<std::pair<int, int>> &neighbours = this->vecini(node);
  for (auto &neighbour : neighbours) {
    if (colors[neighbour.first] == 2)
      return true;
    if (dfs_are_ciclu(neighbour.first, colors)) {
      return true;
    }
    colors[neighbour.first] = 1;
  }
  return false;
}

void Graph::dfs_comp_conexe(int node, std::unordered_set<int> &visited,
                            std::vector<int> &order, bool do_order) {
  visited.insert(node);
  std::vector<std::pair<int, int>> &neighbours = this->vecini(node);
  for (auto &neighbour : neighbours) {
    if (visited.find(neighbour.first) == visited.end()) {
      this->dfs_comp_conexe(neighbour.first, visited, order, do_order);
    }
  }
  if (do_order)
    order.push_back(node);
}

int Graph::comp_conexe() {
  GraphContainer gt_container;
  for (auto &kvpair : this->container) {
    int node1 = kvpair.first;
    for (auto &neighbour : kvpair.second) {
      int node2 = neighbour.first;
      gt_container[node2].push_back({node1, neighbour.second});
    }
  }
  Graph gt(gt_container);
  std::unordered_set<int> visited;
  std::vector<int> order;
  for (auto it = this->container.begin(); it != this->container.end(); it++) {
    if (visited.find(it->first) == visited.end()) {
      this->dfs_comp_conexe(it->first, visited, order);
    }
  }
  visited.clear();
  int count = 0;
  for (auto it = order.rbegin(); it != order.rend(); it++) {
    if (visited.find(*it) == visited.end()) {
      count++;
      gt.dfs_comp_conexe(*it, visited, order, false);
    }
  }
  return count;
}

bool Graph::are_ciclu() {
  std::unordered_map<int, int> colors;
  for (auto &kvpair : this->container) {
    colors[kvpair.first] = 0;
  }
  for (auto it = colors.begin(); it != colors.end(); it++) {
    if (!it->second && dfs_are_ciclu(it->first, colors)) {
      return true;
    }
  }
  return false;
}

bool Graph::este_conex() { return this->comp_conexe() == 1; }

// NU se aplica grafurilor ciclice !!!
std::vector<int> Graph::sortare_topologica() {
  std::vector<int> sorted;
  std::unordered_map<int, int> incoming;
  std::unordered_set<int> no_incoming;
  for (auto &kvpair : this->container) {
    if (incoming.find(kvpair.first) == incoming.end()) {
      no_incoming.insert(kvpair.first);
      incoming[kvpair.first] = 0;
    }
    for (auto &neighbour : kvpair.second) {
      incoming[neighbour.first]++;
      if (no_incoming.find(neighbour.first) != no_incoming.end()) {
        no_incoming.erase(neighbour.first);
      }
    }
  }
  while (!no_incoming.empty()) {
    auto first = no_incoming.begin();
    int node = *first;
    no_incoming.erase(first);
    sorted.push_back(node);
    std::vector<std::pair<int, int>> &neighbours = this->vecini(node);
    for (auto &neighbour : neighbours) {
      if (--incoming[neighbour.first] == 0) {
        no_incoming.insert(neighbour.first);
      }
    }
  }
  return sorted;
}
