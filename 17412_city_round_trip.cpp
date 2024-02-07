// https://www.acmicpc.net/problem/17412

using namespace std;

#include <iostream>
#include <limits>
#include <queue>
#include <vector>

constexpr int MAX_N = 401;
constexpr int INF = numeric_limits<int>::max();
constexpr int SRC = 1;
constexpr int SINK = 2;

vector<int> graph[MAX_N];
int capacity[MAX_N][MAX_N];
int flow[MAX_N][MAX_N];

int N, P;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> N >> P;

  for (int i = 0; i < P; ++i) {
    int from, to;
    cin >> from >> to;
    graph[from].push_back(to);
    graph[to].push_back(from);
    capacity[from][to] = 1;
  }

  int max_flow = 0;

  // Edmond-Karp Algorithm
  while (true) {
    vector<int> parent(N + 1, -1);
    queue<int> q;
    q.push(SRC);

    while (!q.empty()) {
      const int cur = q.front();
      q.pop();

      for (int next: graph[cur]) {
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

  cout << max_flow;
}
