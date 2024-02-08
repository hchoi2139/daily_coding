// https://www.acmicpc.net/problem/11378

#include <algorithm>
#include <cstring>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

using namespace std;

constexpr int INF = numeric_limits<int>::max();
constexpr int MAX_N = 1001;
constexpr int MAX_M = 1001;
constexpr int EMPLOYEE_TASK_OFFSET= 1000;

constexpr int SRC = 2000;
constexpr int SINK = 2001;
constexpr int BRIDGE = 2002;

constexpr int MAX_RESERVED_IDX = 2005;

vector<int> graph[MAX_RESERVED_IDX];
int capacity[MAX_RESERVED_IDX][MAX_RESERVED_IDX];
int flow[MAX_RESERVED_IDX][MAX_RESERVED_IDX];

int N, M, K;

int edmond_karp() {
  int max_flow = 0;

  while (true) {
    int parent[MAX_RESERVED_IDX];
    memset(parent, -1, sizeof parent);

    queue<int> q;
    q.push(SRC);

    while (!q.empty()) {
      const int cur = q.front();
      q.pop();

      for (int next : graph[cur]) {
        if (capacity[cur][next] - flow[cur][next] > 0 && parent[next] == -1) {
          q.push(next);
          parent[next] = cur;
          if (next == SINK) {
            break;
          }
        }
      }
    }

    if (parent[SINK] == -1) {
      break;
    }

    int add_flow = INF;

    for (int i = SINK; i != SRC; i = parent[i]) {
      add_flow = min(add_flow, capacity[parent[i]][i] - flow[parent[i]][i]);
    }

    for (int i = SINK; i != SRC; i = parent[i]) {
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

  cin >> N >> M >> K;

  for (int i = 1; i <= N; ++i) {
    int num_graph;
    cin >> num_graph;

    for (int j = 0; j < num_graph; ++j) {
      int task;
      cin >> task;
      task += EMPLOYEE_TASK_OFFSET;
      graph[i].push_back(task);
      graph[task].push_back(i);
      capacity[i][task] = 1;
    }
  }

  // SRC - BRIDGE link.
  graph[SRC].push_back(BRIDGE);
  graph[BRIDGE].push_back(SRC);
  capacity[SRC][BRIDGE] = K;

  // SRC - Employees and BRIDGE - Employees link.
  for (int i = 1; i <= N; ++i) {
    graph[SRC].push_back(i);
    graph[i].push_back(SRC);
    capacity[SRC][i] = 1;

    graph[BRIDGE].push_back(i);
    graph[i].push_back(BRIDGE);
    capacity[BRIDGE][i] = K;
  }

  // Tasks - SINK link.
  for (int j = 1 + EMPLOYEE_TASK_OFFSET; j <= M + EMPLOYEE_TASK_OFFSET; ++j) {
    graph[j].push_back(SINK);
    graph[SINK].push_back(j);
    capacity[j][SINK] = 1;
  }

  cout << edmond_karp() << endl;
}