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
  this->get_edgelist();
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
    if (x == force_take.first && y == force_take.second)
      continue;

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
  std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>,
                      std::greater<std::pair<int, int>>>
      heap;
  dist[src_node] = 0;
  heap.push({0, src_node});

  while (!heap.empty()) {
    int node = heap.top().second;
    int current_dist = heap.top().first;
    heap.pop();

    if (current_dist > dist[node]) {
      continue;
    }

    for (auto &edge : this->container[node]) {
      int n_node = edge.dest;
      int cost = edge.weight;
      if (dist[node] != INT_MAX && dist[node] + cost < dist[n_node]) {
        dist[n_node] = dist[node] + cost;
        heap.push({dist[n_node], n_node});
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
  std::vector<std::vector<int>> reverse_edge_indices(rg.size());

  for (int u = 0; (size_t)u < this->container.size(); ++u) {
    for (const auto &edge : this->container[u]) {
      int v = edge.dest;
      int cap = edge.cap;
      int flow = edge.flow;

      rg[u].push_back({v, edge.weight, flow, cap});

      rg[v].push_back({u, edge.weight, cap - flow, cap});

      reverse_edge_indices[u].push_back(rg[v].size() - 1);
      reverse_edge_indices[v].push_back(rg[u].size() - 1);
    }
  }

  std::vector<std::pair<int, int>> parent(container.size());

  while (true) {
    std::queue<int> q;
    q.push(s);
    std::fill(parent.begin(), parent.end(), std::make_pair(-1, -1));
    parent[s] = {-2, -2};

    bool found_path = false;
    while (!q.empty() && !found_path) {
      int u = q.front();
      q.pop();

      for (int i = 0; (size_t)i < rg[u].size(); ++i) {
        const auto &edge = rg[u][i];
        int v = edge.dest;
        int rez_flow = edge.cap - edge.flow;

        if (parent[v].first == -1 && rez_flow > 0) {
          parent[v] = {u, i};
          q.push(v);
          if (v == t) {
            found_path = true;
            break;
          }
        }
      }
    }

    if (!found_path)
      break;

    int bottleneck = INT_MAX;
    for (int v = t; v != s; v = parent[v].first) {
      int u = parent[v].first;
      int edge_idx = parent[v].second;
      bottleneck =
          std::min(bottleneck, rg[u][edge_idx].cap - rg[u][edge_idx].flow);
    }

    for (int v = t; v != s; v = parent[v].first) {
      int u = parent[v].first;
      int edge_idx = parent[v].second;

      rg[u][edge_idx].flow += bottleneck;

      int rev_idx = reverse_edge_indices[u][edge_idx];
      rg[v][rev_idx].flow -= bottleneck;
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

std::pair<std::vector<int>, std::vector<int>> Graph::bellman_ford(int src) {
  std::vector<int> tata(this->container.size(), -1);
  std::vector<int> d(this->container.size(), INT_MAX);
  std::queue<int> q;
  std::vector<bool> in_queue(this->container.size());
  std::vector<int> ct_in_queue(this->container.size());
  d[src] = 0;
  in_queue[src] = true;
  q.push(src);
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    in_queue[u] = false;
    for (auto &edge : this->container[u]) {
      int v = edge.dest, w = edge.weight;
      if (d[u] < INT_MAX && d[u] + w < d[v]) {
        d[v] = d[u] + w;
        tata[v] = u;
        if (!in_queue[v]) {
          q.push(v);
          in_queue[v] = true;
          ct_in_queue[v]++;
          if (ct_in_queue[v] > this->container.size()) {
            return {};
          }
        }
      }
    }
  }
  return {tata, d};
}

void Graph::floyd_warshall(IntMatrix &d, IntMatrix &p) {
  d.resize(this->size(), std::vector<int>(this->size(), INT_MAX));
  p.resize(this->size(), std::vector<int>(this->size()));
  for (size_t u = 0; u < this->size(); u++) {
    for (auto &edge : this->container[u]) {
      int v = edge.dest, w = edge.weight;
      d[u][v] = w;
      p[u][v] = u;
    }
  }
  int n = this->size();
  for (size_t k = 0; k < n; k++) {
    for (size_t i = 0; i < n; i++) {
      for (size_t j = 0; j < n; j++) {
        if (i == j) {
          d[i][j] = 0;
        }
        if (d[i][k] == INT_MAX || d[k][j] == INT_MAX)
          continue;
        if (d[i][j] > d[i][k] + d[k][j]) {
          d[i][j] = d[i][k] + d[k][j];
          p[i][j] = p[k][j];
        }
      }
    }
  }
}
