// https://www.acmicpc.net/problem/2316

#include <algorithm>
#include <cstring>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

using namespace std;

static constexpr int INF = numeric_limits<int>::max();
static constexpr int SRC_CITY = 1;
static constexpr int SINK_CITY = 2;

// Node starts from index 2 up to 801.
static constexpr int MAX_NODE = 805;

static constexpr int to_in_node(const int city) {
  return city << 1;
}

static constexpr int to_out_node(const int city) {
  return (city << 1) + 1;
}

static constexpr int SRC_OUT = to_out_node(SRC_CITY);
static constexpr int SINK_IN = to_in_node(SINK_CITY);

vector<int> graph[MAX_NODE];
int capacity[MAX_NODE][MAX_NODE];
int flow[MAX_NODE][MAX_NODE];

int num_cities, num_roads;

[[nodiscard]] int edmond_karp() {
  int max_flow = 0;

  while (true) {
    int parent[MAX_NODE];
    memset(parent, -1, sizeof parent);

    queue<int> q;
    q.push(SRC_OUT);

    while (!q.empty()) {
      const int cur = q.front();
      q.pop();

      for (int next : graph[cur]) {
        if (capacity[cur][next] - flow[cur][next] > 0 && parent[next] == -1) {
          q.push(next);
          parent[next] = cur;
          if (next == SINK_IN) {
            break;
          }
        }
      }
    }

    if (parent[SINK_IN] == -1) {
      break;
    }

    int add_flow = INF;

    for (int i = SINK_IN; i != SRC_OUT; i = parent[i]) {
      add_flow = min(add_flow, capacity[parent[i]][i] - flow[parent[i]][i]);
    }

    for (int i = SINK_IN; i != SRC_OUT; i = parent[i]) {
      flow[parent[i]][i] += add_flow;
      flow[i][parent[i]] -= add_flow;
    }

    max_flow += add_flow;
  }

  return max_flow;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> num_cities >> num_roads;

  // Connect internal in-nodes and out-nodes.
  for (int c = 1; c <= num_cities; ++c) {
    const int in_node = to_in_node(c);
    const int out_node = to_out_node(c);

    graph[in_node].push_back(out_node);
    graph[out_node].push_back(in_node);
    capacity[in_node][out_node] = 1;
  }

  // Connect inter-city roads.
  for (int i = 0; i < num_roads; ++i) {
    int from, to;
    cin >> from >> to;

    const int from_in = to_in_node(from);
    const int from_out = to_out_node(from);
    const int to_in = to_in_node(to);
    const int to_out = to_out_node(to);

    // Connect from and to.
    graph[from_out].push_back(to_in);
    graph[to_in].push_back(from_out);
    capacity[from_out][to_in] = 1;

    graph[to_out].push_back(from_in);
    graph[from_in].push_back(to_out);
    capacity[to_out][from_in] = 1;
  }

  const int max_flow = edmond_karp();
  cout << max_flow;

  return 0;
}