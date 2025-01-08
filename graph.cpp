#include "graph.h"
#include "limits.h"
#include <algorithm>
#include <iostream>
#include <queue>
#include <set>

void Graph::print_graph() {
  for (size_t node = 0; node < this->container.size(); node++) {
    std::cout << "Node " << node << ": ";
    for (auto &neighbor : this->container[node]) {
      std::cout << "(" << neighbor.first << ", " << neighbor.second << ") ";
    }
    std::cout << std::endl;
  }
}

bool Graph::dfs_are_ciclu(int node, std::vector<int> &colors) {
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

void Graph::dfs_comp_conexe(int node, std::vector<int> &visited,
                            std::vector<int> &order, bool do_order) {
  visited[node] = 1;
  std::vector<std::pair<int, int>> &neighbours = this->vecini(node);
  for (auto &neighbour : neighbours) {
    if (!visited[neighbour.first]) {
      this->dfs_comp_conexe(neighbour.first, visited, order, do_order);
    }
  }
  if (do_order)
    order.push_back(node);
}

int Graph::comp_conexe() {
  GraphContainer gt_container;
  gt_container.resize(this->container.size());
  for (size_t node1 = 0; node1 < this->container.size(); node1++) {
    for (auto &neighbour : this->container[node1]) {
      int node2 = neighbour.first;
      gt_container[node2].push_back({node1, neighbour.second});
    }
  }
  Graph gt(gt_container);
  std::vector<int> visited;
  visited.resize(this->container.size());
  std::vector<int> order;
  visited.resize(this->container.size(), 0);
  for (size_t node = 0; node < this->container.size(); node++) {
    if (!visited[node]) {
      this->dfs_comp_conexe(node, visited, order);
    }
  }
  std::fill(visited.begin(), visited.end(), 0);
  int count = 0;
  for (auto it = order.rbegin(); it != order.rend(); it++) {
    if (!visited[*it]) {
      count++;
      gt.dfs_comp_conexe(*it, visited, order, false);
    }
  }
  return count;
}

bool Graph::are_ciclu() {
  std::vector<int> colors;
  colors.resize(this->container.size(), 0);

  for (size_t node = 0; node < colors.size(); node++) {
    if (!colors[node] && dfs_are_ciclu(node, colors)) {
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
  for (size_t node = 0; node < this->container.size(); node++) {
    if (incoming.find(node) == incoming.end()) {
      no_incoming.insert(node);
      incoming[node] = 0;
    }
    for (auto &neighbour : this->container[node]) {
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

Graph Graph::neorientat() {
  Graph neorientat = *this;
  for (size_t node = 0; node < neorientat.container.size(); node++) {
    for (auto neighbour : this->container[node]) {
      neorientat.container[neighbour.first].push_back({node, neighbour.second});
    }
  }
  return neorientat;
}

bool Graph::este_bipartit() {
  std::unordered_map<int, int> colors;
  std::queue<int> q;
  Graph neorientat = this->neorientat();
  for (size_t node = 0; node < neorientat.container.size(); node++) {
    if (colors[node] == 0) {
      q.push(node);
      colors[node] = 1;
      while (!q.empty()) {
        int current_node = q.front();
        q.pop();
        int color = colors[current_node];
        std::vector<std::pair<int, int>> &neighbours =
            neorientat.vecini(current_node);
        for (auto &neighbour : neighbours) {
          if (colors[neighbour.first] == color) {
            return false;
          } else if (colors[neighbour.first] == 0) {
            q.push(neighbour.first);
            colors[neighbour.first] = color == 1 ? 2 : 1;
          }
        }
      }
    }
  }
  return true;
}

DSU::DSU(int n) {
  this->parent.resize(n);
  this->rank.resize(n, 0);
  for (int i = 0; i < n; ++i) {
    this->parent[i] = i;
  }
}

int DSU::find(int x) {
  if (this->parent[x] != x) {
    this->parent[x] = find(parent[x]);
  }
  return this->parent[x];
}

void DSU::unite(int x, int y) {
  int rootX = find(x);
  int rootY = find(y);
  if (rootX != rootY) {
    if (this->rank[rootX] > rank[rootY]) {
      this->parent[rootY] = rootX;
    } else if (this->rank[rootX] < rank[rootY]) {
      this->parent[rootX] = rootY;
    } else {
      this->parent[rootY] = rootX;
      this->rank[rootX]++;
    }
  }
}

void Graph::get_edgelist() {
  for (size_t node = 0; node < this->container.size(); node++) {
    for (auto &neighbour : this->container[node]) {
      this->edge_list.push_back({neighbour.second, node, neighbour.first});
    }
  }
}

Graph Graph::kruskal() {
  std::sort(this->edge_list.begin(), this->edge_list.end());

  GraphContainer container;
  container.resize(this->container.size());
  DSU s(this->container.size());
  size_t count = 0;
  for (auto &edge : this->edge_list) {
    int w = std::get<0>(edge);
    int x = std::get<1>(edge);
    int y = std::get<2>(edge);

    if (s.find(x) != s.find(y)) {
      s.unite(x, y);
      container[x].push_back({y, w});
      count++;
    }
    if (count == this->container.size() - 1) {
      break;
    }
  }
  return Graph(container);
}

int Graph::greutate() {
  int sum = 0;
  for (auto &edge : this->edge_list) {
    sum += std::get<0>(edge);
  }
  return sum;
}

void Graph::dfs_puncte_critice(int node, int parent, std::vector<bool> &visited,
                               std::vector<int> &disc, std::vector<int> &low,
                               int &time, std::vector<bool> &critical) {
  visited[node] = true;
  disc[node] = low[node] = ++time;

  int children = 0;
  for (std::pair<int, int> &neighbour : this->container[node]) {
    int n_node = neighbour.first;
    if (!visited[n_node]) {
      children++;
      dfs_puncte_critice(n_node, node, visited, disc, low, time, critical);

      low[node] = std::min(low[node], low[n_node]);

      if (parent != -1 && low[n_node] >= disc[node]) {
        critical[node] = true;
      }
    } else if (n_node != parent) {
      low[node] = std::min(low[node], disc[n_node]);
    }
  }

  if (parent == -1 && children > 1) {
    critical[node] = true;
  }
}

std::vector<int> Graph::puncte_critice() {
  size_t n = this->container.size();
  std::vector<int> disc(n, 0), low(n, 0);
  std::vector<bool> visited(n, false), critical(n, false);
  int time = 0;

  for (size_t node = 0; node < this->container.size(); node++) {
    if (!visited[node]) {
      dfs_puncte_critice(node, -1, visited, disc, low, time, critical);
    }
  }

  std::vector<int> nodes;
  for (size_t i = 0; i < critical.size(); i++) {
    if (critical[i])
      nodes.push_back(i);
  }

  return nodes;
}

std::vector<int> Graph::dijkstra(int src_node) {
  size_t n = this->container.size();
  std::vector<int> dist(n, INT_MAX);
  std::set<std::pair<int, int>> heap;
  dist[src_node] = 0;
  heap.insert({0, src_node});

  while (!heap.empty()) {
    int node = heap.begin()->second;
    heap.erase(heap.begin());

    for (std::pair<int, int> &neighbour : this->container[node]) {
      int n_node = neighbour.first;
      int cost = neighbour.second;
      if (dist[node] + cost < dist[n_node] && dist[node] != INT_MAX) {
        dist[n_node] = dist[node] + cost;
        heap.insert({dist[n_node], n_node});
      }
    }
  }
  return dist;
}
