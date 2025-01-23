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
      std::cout << "(" << neighbor.dest << ", " << neighbor.weight << ", "
                << neighbor.flow << ", " << neighbor.cap << ") ";
    }
    std::cout << std::endl;
  }
}

bool Graph::dfs_are_ciclu(int node, std::vector<int> &colors) {
  colors[node] = 2;
  auto &edges = this->edges(node);
  for (auto &edge : edges) {
    if (colors[edge.dest] == 2)
      return true;
    if (dfs_are_ciclu(edge.dest, colors)) {
      return true;
    }
    colors[edge.dest] = 1;
  }
  return false;
}

void Graph::dfs_comp_conexe(int node, std::vector<int> &visited,
                            std::vector<int> &order, bool do_order) {
  visited[node] = 1;
  auto &edges = this->edges(node);
  for (auto &edge : edges) {
    if (!visited[edge.dest]) {
      this->dfs_comp_conexe(edge.dest, visited, order, do_order);
    }
  }
  if (do_order)
    order.push_back(node);
}

int Graph::comp_conexe() {
  GraphContainer gt_container;
  gt_container.resize(this->container.size());
  for (size_t node1 = 0; node1 < this->container.size(); node1++) {
    for (auto &edge : this->container[node1]) {
      int node2 = edge.dest;
      gt_container[node2].push_back(
          {(int)node1, edge.weight, edge.flow, edge.cap});
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
    for (auto &edge : this->container[node]) {
      incoming[edge.dest]++;
      if (no_incoming.find(edge.dest) != no_incoming.end()) {
        no_incoming.erase(edge.dest);
      }
    }
  }
  while (!no_incoming.empty()) {
    auto dest = no_incoming.begin();
    int node = *dest;
    no_incoming.erase(dest);
    sorted.push_back(node);
    auto &edges = this->edges(node);
    for (auto &edge : edges) {
      if (--incoming[edge.dest] == 0) {
        no_incoming.insert(edge.dest);
      }
    }
  }
  return sorted;
}

Graph Graph::neorientat() {
  GraphContainer c(this->container.size());
  std::vector<std::unordered_set<int>> g(this->container.size());
  for (int node = 0; (size_t)node < this->container.size(); node++) {
    for (auto &edge : this->container[node]) {
      if (g[edge.dest].find(node) == g[edge.dest].end()) {
        g[edge.dest].insert(node);
        c[edge.dest].push_back({node, edge.weight, edge.flow, edge.cap});
      }
      if (g[node].find(edge.dest) == g[node].end()) {
        g[node].insert(edge.dest);
        c[node].push_back(edge);
      }
    }
  }
  return Graph(c);
}

std::unordered_map<int, int> Graph::este_bipartit() {
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
        auto &edges = neorientat.edges(current_node);
        for (auto &edge : edges) {
          if (colors[edge.dest] == color) {
            return {{-1, -1}};
          } else if (colors[edge.dest] == 0) {
            q.push(edge.dest);
            colors[edge.dest] = color == 1 ? 2 : 1;
          }
        }
      }
    }
  }
  return colors;
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
    for (auto &edge : this->container[node]) {
      this->edge_list.push_back({edge.weight, node, edge.dest});
    }
  }
}

Graph Graph::kruskal(std::pair<int, int> force_take) {
  std::sort(this->edge_list.begin(), this->edge_list.end());

  GraphContainer container;
  container.resize(this->container.size());
  DSU s(this->container.size());
  size_t count = 0;
  if (force_take.first != -1) {
    int x = force_take.first;
    int y = this->container[x][force_take.second].dest;
    int w = this->container[x][force_take.second].weight;
    s.unite(x, y);
    container[x].push_back({y, w});
    count++;
  }
  if (count == this->container.size() - 1)
    return Graph(container);
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
  for (auto &edge : this->container[node]) {
    int n_node = edge.dest;
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

    for (auto &edge : this->container[node]) {
      int n_node = edge.dest;
      int cost = edge.weight;
      if (dist[node] + cost < dist[n_node] && dist[node] != INT_MAX) {
        dist[n_node] = dist[node] + cost;
        heap.insert({dist[n_node], n_node});
      }
    }
  }
  return dist;
}

void Graph::add_edge(int node1, int node2, int weight = 0, int flow = 0,
                     int cap = 0) {
  this->container[node1].push_back({node2, weight, flow, cap});
}

void Graph::remove_edge(int node1, int node2) {
  auto edges = this->edges(node1);
  auto it = std::find(edges.begin(), edges.end(), node2);
  if (it != this->container[node1].end()) {
    this->container[node1].erase(it);
  }
}

bool operator==(Edge &e1, const int node) { return e1.dest == node; }

int Graph::edmonds_karp(int s, int t) {
  int max_flow = 0;
  GraphContainer rg(this->container.size());
  for (int node = 0; (size_t)node < this->container.size(); node++) {
    for (auto &edge : this->container[node]) {
      rg[node].push_back({edge.dest, edge.weight, edge.flow, edge.cap});
      rg[edge.dest].push_back(
          {node, edge.weight, edge.cap - edge.flow, edge.cap});
    }
  }

  bool path = true;
  std::vector<int> parent(this->container.size());
  while (path) {
    path = false;
    std::queue<int> q;
    std::vector<bool> visited(this->container.size());

    // BFS

    q.push(s);
    visited[s] = true;
    while (!q.empty()) {
      int node = q.front();
      q.pop();

      for (auto &edge : rg[node]) {
        int rez_flow = edge.cap - edge.flow;
        if (!visited[edge.dest] && rez_flow > 0) {
          visited[edge.dest] = true;
          parent[edge.dest] = node;
          q.push(edge.dest);
          if (edge.dest == t) {
            path = true;
            break;
          }
        }
      }
    }
    if (!path)
      break;

    int current = t;
    int bottleneck = INT_MAX;
    while (current != s) {
      int prev = parent[current];
      for (auto &edge : rg[prev]) {
        if (edge.dest == current && edge.cap - edge.flow > 0) {
          bottleneck = std::min(bottleneck, edge.cap - edge.flow);
          break;
        }
      }
      current = prev;
    };

    current = t;
    while (current != s) {
      int prev = parent[current];
      for (auto &edge : rg[prev]) {
        if (edge.dest == current && edge.flow + bottleneck <= edge.cap) {
          edge.flow += bottleneck;
          break;
        }
      }
      for (auto &edge : rg[current]) {
        if (edge.dest == prev && edge.flow - bottleneck >= 0) {
          edge.flow -= bottleneck;
          break;
        }
      }
      current = prev;
    }

    max_flow += bottleneck;
  }

  return max_flow;
}

int Graph::cuplu_max() {
  auto colors = this->este_bipartit();
  if (colors.find(-1) != colors.end()) {
    return 0;
  }

  Graph flux = *this;
  flux.add_node();
  int s = flux.size() - 1;
  flux.add_node();
  int t = s + 1;

  for (auto node_color : colors) {
    int node = node_color.first;
    int color = node_color.second;
    if (color == 1) {
      flux.add_edge(s, node, 0, 0, 1);
    } else {
      flux.add_edge(node, t, 0, 0, 1);
    }
  }
  for (auto &edges : flux.container) {
    for (auto &edge : edges) {
      edge.cap = 1;
      edge.flow = 0;
    }
  }
  return flux.edmonds_karp(s, t);
}

void Graph::add_node() { this->container.push_back({}); }

size_t Graph::size() const { return this->container.size(); }
