#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <set>

struct Edge {
  int64_t to;
  int64_t weight;
};

struct FullEdge {
  int64_t from;
  int64_t to;
  int64_t weight;
};

struct FlowEdge {
  int64_t to;
  int64_t cap;
  int64_t flow;
};

struct TimeEdge {
  int64_t to;
  int64_t departure;
  int64_t arrival;
};

using Graph = std::vector<std::vector<int64_t>>;
using WeightGraph = std::vector<std::vector<Edge>>;
using BWeightGraph = std::vector<std::vector<FullEdge>>;
using BPair = std::pair<int64_t, std::pair<int64_t, int64_t>>;
using FlowGraph = std::vector<std::vector<FlowEdge>>;
using TimeGraph = std::vector<std::vector<TimeEdge>>;
using VPair = std::vector<std::pair<int64_t, int64_t>>;

struct DSU {
  std::vector<int64_t> rank;
  std::vector<int64_t> parent;
  std::vector<int64_t> components;
  explicit DSU(int64_t n) {
    rank.resize(n);
    parent.resize(n);
    components.resize(n);
    for (auto i = 0; i < n; ++i) {
      parent[i] = i;
      rank[i] = 1;
      components[i] = 0;
    }
  }
  DSU(const std::vector<int64_t>& r, const std::vector<int64_t>& p) : rank(r), parent(p) {
  }
  DSU(const std::vector<int64_t>& r, const std::vector<int64_t>& p, const std::vector<int64_t>& c)
      : rank(r), parent(p), components(c) {
  }
  void MakeSet() {
    parent.emplace_back(parent.size());
    rank.emplace_back(1);
  }
  int64_t FindSet(int64_t x) {
    if (x == parent[x]) {
      return x;
    }
    return (parent[x] = FindSet(parent[x]));
  }
  void Union(int64_t x, int64_t y) {
    x = FindSet(x);
    y = FindSet(y);

    if (rank[x] < rank[y]) {
      parent[x] = y;
    } else if (rank[x] > rank[y]) {
      parent[y] = x;
    } else {
      parent[x] = y;
      ++rank[y];
    }
    /*
    components[x] += components[y];
    components[y] = components[x];
     */
  }
  int64_t SetCount() {
    std::set<int64_t> set_cnt;
    for (auto i : parent) {
      set_cnt.insert(FindSet(i));
    }
    return set_cnt.size();
  }
};

bool Cmp(FullEdge first, FullEdge second) {
  return first.weight < second.weight;
}

int64_t Prim(std::vector<int64_t>& used, const WeightGraph& graph, Graph& mst, int64_t n, int64_t inf) {
  std::vector<int64_t> dist(n, inf);
  std::vector<int64_t> prev(n, -1);
  std::priority_queue<std::pair<int64_t, int64_t>, std::vector<std::pair<int64_t, int64_t>>, std::greater<>> q;
  dist[0] = 0;
  q.emplace(0, 0);
  while (!q.empty()) {
    auto [d, p] = q.top();
    q.pop();
    if (used[p]) {
      continue;
    }
    used[p] = true;
    if (prev[p] != -1) {
      mst[prev[p]].push_back(p);
      mst[p].push_back(prev[p]);
    }
    for (auto& e : graph[p]) {
      if (!used[e.to] && e.weight < dist[e.to]) {
        prev[e.to] = p;
        dist[e.to] = e.weight;
        q.emplace(dist[e.to], e.to);
      }
    }
  }
  int64_t mst_weight = 0;
  for (int64_t i = 0; i < n; ++i) {
    if (dist[i] != inf) {
      mst_weight += dist[i];
    }
  }
  return mst_weight;
}

void Kruskal(std::vector<FullEdge>& graph, std::vector<FullEdge>& mst, int64_t n) {
  std::vector<int64_t> rank(n, 1);
  std::vector<int64_t> parent(n);
  for (auto i = 0; i < n; ++i) {
    parent[i] = i;
  }
  std::sort(graph.begin(), graph.end(), Cmp);
  DSU dsu(rank, parent);
  for (auto& e : graph) {
    if (dsu.FindSet(e.from) != dsu.FindSet(e.to)) {
      dsu.Union(e.from, e.to);
      mst.push_back({e.from, e.to, e.weight});
    }
  }
}

void Boruvka(const std::vector<FullEdge>& graph, std::vector<FullEdge>& mst, int64_t n, int64_t inf) {
  int64_t ans;
  std::vector<int64_t> rank(n, 1);
  std::vector<int64_t> parent(n);
  for (auto i = 0; i < n; ++i) {
    parent[i] = i;
  }
  DSU dsu(rank, parent);
  while (dsu.SetCount() > 1) {
    std::vector<FullEdge> min_edge(n);
    for (auto& j : min_edge) {
      j.weight = inf;
    }
    for (auto& e : graph) {
      auto first_component = dsu.FindSet(e.from);
      auto second_component = dsu.FindSet(e.to);
      if (first_component != second_component) {
        if (e.weight < min_edge[first_component].weight) {
          min_edge[first_component] = {e.to, e.from, e.weight};
          if (e.weight < min_edge[second_component].weight) {
            min_edge[second_component] = {e.to, e.from, e.weight};
          }
        }
      }
    }
    for (auto& i : min_edge) {
      if (i.weight == inf) {
        continue;
      }
      if (dsu.FindSet(i.to) != dsu.FindSet(i.from)) {
        mst.push_back(i);
      }
      dsu.Union(i.to, i.from);
    }
  }
}

std::vector<int64_t> BellmanFord(int64_t start, int64_t n, int64_t inf, const std::vector<FullEdge>& graph) {
  std::vector<int64_t> dist(n, inf);
  std::vector<int64_t> parent(n, -1);
  // bool has_negative_cycle = false;
  dist[start] = 0;
  for (int64_t i = 0; i < n - 1; ++i) {
    for (auto& e : graph) {
      if (dist[e.to] > dist[e.from] + e.weight && e.weight != inf) {
        dist[e.to] = dist[e.from] + e.weight;
        parent[e.to] = e.from;
      }
    }
  }
  std::vector<int64_t> negative_cycle;
  for (auto& e : graph) {
    if (dist[e.to] > dist[e.from] + e.weight && e.weight != inf) {
      parent[e.to] = e.from;
      int64_t cycle_start = e.to;
      // we need to get to the negative cycle
      for (auto i = 0; i < n; ++i) {
        cycle_start = parent[cycle_start];
      }
      for (auto cur = cycle_start; cur < n; cur = parent[cur]) {
        negative_cycle.push_back(cur);
        if (cur == cycle_start && negative_cycle.size() > 1) {
          break;
        }
      }
      return negative_cycle;
    }
  }

  return negative_cycle;
}

std::vector<int64_t> BellmanFord(int64_t start, int64_t n, int64_t inf, const Graph& graph) {
  std::vector<int64_t> dist(n, inf);
  std::vector<int64_t> parent(n, -1);
  dist[start] = 0;
  for (int64_t i = 0; i < n - 1; ++i) {
    for (int64_t j = 0; j < n; ++j) {
      for (int64_t k = 0; k < n; ++k) {
        if (dist[k] > graph[j][k] + dist[j] && graph[j][k] != inf) {
          dist[k] = graph[j][k] + dist[j];
          parent[k] = j;
        }
      }
    }
  }
  std::vector<int64_t> negative_cycle;
  for (int64_t j = 0; j < n; ++j) {
    for (int64_t k = 0; k < n; ++k) {
      if (dist[k] > graph[j][k] + dist[j] && graph[j][k] != inf) {
        parent[k] = j;
        int64_t cycle_start = k;
        for (int64_t i = 0; i < n; ++i) {
          cycle_start = parent[cycle_start];
        }
        for (auto current = cycle_start; current < n; current = parent[current]) {
          negative_cycle.push_back(current);
          if (current == cycle_start && negative_cycle.size() > 1) {
            break;
          }
        }
      }
    }
  }
  return negative_cycle;
}

std::vector<int64_t> BellmanFord(int64_t start, int64_t n, int64_t inf, const WeightGraph& graph) {
  std::vector<int64_t> dist(n, inf);
  dist[start] = 0;
  for (int64_t i = 0; i < n - 1; ++i) {
    for (int64_t j = 0; j < n; ++j) {
      for (auto to : graph[j]) {
        if (dist[to.to] > dist[j] + to.weight && dist[j] != inf) {
          dist[to.to] = dist[j] + to.weight;
        }
      }
    }
  }
  return dist;
}

std::vector<std::vector<int64_t>> Dijkstra(int64_t start, int64_t n, int64_t k, int64_t inf, const WeightGraph& graph) {
  std::vector<std::vector<int64_t>> dist(n, std::vector<int64_t>(k + 1, inf));
  dist[start][0] = 0;
  std::priority_queue<BPair, std::vector<BPair>, std::greater<>> q;
  q.push({0, {start, 0}});
  while (!q.empty()) {
    auto [d, v] = q.top();
    q.pop();
    if (dist[v.first][v.second] != d) {
      continue;
    }
    for (auto& u : graph[v.first]) {
      if (v.second + 1 <= k && dist[u.to][v.second + 1] > dist[v.first][v.second] + u.weight) {
        dist[u.to][v.second + 1] = dist[v.first][v.second] + u.weight;
        q.push({dist[u.to][v.second + 1], {u.to, v.second + 1}});
      }
    }
  }
  return dist;
}

std::vector<int64_t> Dijkstra(int64_t start, int64_t n, std::vector<int64_t>& used, const WeightGraph& graph) {
  const int64_t inf = 2009000999;
  std::vector<int64_t> dist(n, inf);
  dist[start] = 0;
  std::priority_queue<std::pair<int64_t, int64_t>, std::vector<std::pair<int64_t, int64_t>>, std::greater<>> q;
  q.emplace(0, start);
  while (!q.empty()) {
    auto [d, v] = q.top();
    q.pop();
    if (used[v]) {
      continue;
    }
    used[v] = true;
    for (auto& u : graph[v]) {
      if (dist[u.to] > dist[v] + u.weight) {
        dist[u.to] = dist[v] + u.weight;
        q.emplace(dist[u.to], u.to);
      }
    }
  }
  return dist;
}

int64_t Johnson(int64_t begin, int64_t n, int64_t inf, WeightGraph& graph) {
  auto shortest_dist = BellmanFord(begin, n + 1, inf, graph);
  // WeightGraph g(n);
  for (auto i = 0; i < n; ++i) {
    for (auto& e : graph[i]) {
      e.weight += (shortest_dist[i] - shortest_dist[e.to]);
      // g[i].push_back({e.to, e.weight});
    }
  }
  int64_t max_dist = -inf;
  std::pair<int64_t, int64_t> potential;
  for (auto i = 0; i < n; ++i) {
    std::vector<int64_t> used(n, 0);
    auto johnson_dist = Dijkstra(i, n, used, graph);
    for (auto j = 0; j < n; ++j) {
      if (johnson_dist[j] != inf) {
        if (max_dist < johnson_dist[j]) {
          max_dist = johnson_dist[j];
          potential.first = j;
          potential.second = i;
        }
      }
    }
  }
  return (max_dist + shortest_dist[potential.first] - shortest_dist[potential.second]);
}

void FloidWarshall(int64_t n, Graph& graph) {
  for (int64_t k = 0; k < n; ++k) {
    for (int64_t i = 0; i < n; ++i) {
      for (int64_t j = 0; j < n; ++j) {
        graph[i][j] = std::min(graph[i][k] + graph[k][j], graph[i][j]);
      }
    }
  }
}

int64_t DFS(int64_t start, int64_t finish, int64_t flow, std::vector<int64_t>& used, FlowGraph& graph) {
  if (start == finish) {
    return flow;
  }
  used[start] = true;
  for (auto& u : graph[start]) {
    if (!used[u.to] && u.flow < u.cap) {
      int64_t f = DFS(u.to, finish, std::min(flow, u.cap - u.flow), used, graph);
      if (f > 0) {
        u.flow += f;
        for (auto& e : graph[u.to]) {
          if (e.to == start) {
            e.flow -= f;
            break;
          }
        }
        return f;
      }
    }
  }
  return 0;
}

int64_t FordFulkerson(FlowGraph& graph, int64_t start, int64_t finish, int64_t n, int64_t inf) {
  int64_t max_flow = 0;
  int64_t cur_flow = -1;
  std::vector<int64_t> used(n, 0);
  while (cur_flow != 0) {
    cur_flow = DFS(start, finish, inf, used, graph);
    max_flow += cur_flow;
    std::fill(used.begin(), used.end(), 0);
  }
  return max_flow;
}

bool BFS(Graph& graph, Graph& flow, std::vector<int64_t>& level, int64_t n, int64_t s, int64_t f, int64_t inf) {
  level[s] = 0;
  std::queue<int64_t> q;
  q.push(s);
  while (!q.empty()) {
    int64_t v = q.front();
    q.pop();
    for (int64_t e = 0; e < n; ++e) {
      if (level[e] == -1 && flow[v][e] < graph[v][e]) {
        level[e] = level[v] + 1;
        q.push(e);
      }
    }
  }
  return (level[f] != -1);
}

int64_t DFS(int64_t n, int64_t start, int64_t finish, int64_t cur_flow, std::vector<int64_t>& level, Graph& graph, Graph& flow) {
  if (start == finish) {
    return cur_flow;
  }
  for (auto u = 0; u < n; ++u) {
    if ((level[u] == level[start] + 1) && flow[start][u] < graph[start][u]) {
      int64_t f = DFS(n, u, finish, std::min(cur_flow, graph[start][u] - flow[start][u]), level, graph, flow);
      if (f > 0) {
        flow[start][u] += f;
        flow[u][start] -= f;
        return f;
      }
    }
  }
  return 0;
}

int64_t FordFulkerson(Graph& graph, Graph& flow, int64_t start, int64_t finish, int64_t n, int64_t inf) {
  int64_t max_flow = 0;
  int64_t cur_flow = -1;
  std::vector<int64_t> used(n, 0);
  while (cur_flow != 0) {
    cur_flow = DFS(n, start, finish, inf, used, graph, flow);
    max_flow += cur_flow;
    std::fill(used.begin(), used.end(), 0);
  }
  return max_flow;
}

int64_t EdmonsKarp(Graph& graph, Graph& flow, int64_t start, int64_t finish, int64_t n, int64_t inf) {
  int64_t max_flow = 0;
  std::vector<int64_t> parent(n, -1);
  while (BFS(graph, flow, parent, n, start, finish, inf)) {
    int64_t cur_flow = inf;
    for (auto i = finish; i != start; i = parent[i]) {
      cur_flow = std::min(cur_flow, graph[parent[i]][i] - flow[parent[i]][i]);
    }
    for (auto i = finish; i != start; i = parent[i]) {
      flow[parent[i]][i] += cur_flow;
      flow[i][parent[i]] -= cur_flow;
    }
    max_flow += cur_flow;
    std::fill(parent.begin(), parent.end(), -1);
  }
  return max_flow;
}

int64_t Dinic(Graph& graph, Graph& flow, int64_t sourse, int64_t sink, int64_t n, int64_t inf) {
  int64_t max_flow = 0;
  std::vector<int64_t> level(n, -1);
  while (BFS(graph, flow, level, n, sourse, sink, inf)) {
    int64_t cur_flow = 0;
    while ((cur_flow = DFS(n, sourse, sink, inf, level, graph, flow)) > 0) {
      max_flow += cur_flow;
    }
    std::fill(level.begin(), level.end(), -1);
  }
  return max_flow;
}

int64_t Makhovic(const TimeGraph& graph, int64_t start, int64_t finish, int64_t n, int64_t inf) {
  std::vector<int64_t> time_dist(n, inf);
  time_dist[start] = 0;
  std::priority_queue<std::pair<int64_t, int64_t>, std::vector<std::pair<int64_t, int64_t>>, std::greater<>> q;
  q.emplace(0, start);
  while (!q.empty()) {
    auto [time, v] = q.top();
    q.pop();
    for (auto& e : graph[v]) {
      if (e.departure >= time && e.arrival < time_dist[e.to]) {
        time_dist[e.to] = e.arrival;
        q.emplace(time_dist[e.to], e.to);
      }
    }
  }
  return time_dist[finish];
}

std::vector<int64_t> BellmanFord(int64_t start, int64_t n, int64_t inf, VPair& parent, const BWeightGraph& graph) {
  std::vector<int64_t> dist(n, inf);
  dist[start] = 0;
  for (int64_t i = 0; i < n - 1; ++i) {
    for (int64_t j = 0; j < n; ++j) {
      for (auto to : graph[j]) {
        if (dist[to.to] > dist[j] + to.weight && dist[j] != inf) {
          dist[to.to] = dist[j] + to.weight;
          parent[to.to] = {j, to.from};
        }
      }
    }
  }
  for (int64_t j = 0; j < n; ++j) {
    for (auto to : graph[j]) {
      if (dist[to.to] > dist[j] + to.weight && dist[j] != inf) {
        std::vector<int64_t> negative_cycle;
        parent[to.to] = {j, to.from};
        int64_t cycle_start = to.to;
        for (int64_t i = 0; i < n; ++i) {
          cycle_start = parent[cycle_start].first;
        }
        for (auto current = cycle_start; current < n; current = parent[current].first) {
          negative_cycle.push_back(current);
          if (current == cycle_start && negative_cycle.size() > 1) {
            break;
          }
        }
        return negative_cycle;
      }
    }
  }
  return {};
}
