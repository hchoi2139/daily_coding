// https://www.acmicpc.net/problem/13161

#include <algorithm>
#include <cstring>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

using namespace std;

static constexpr int INF = numeric_limits<int>::max();

static constexpr int SRC = 501;
static constexpr int SINK = 502;

static constexpr int MAX_IDX = 505;

vector<int> graph[MAX_IDX];

int cap[MAX_IDX][MAX_IDX];
int flow[MAX_IDX][MAX_IDX];

int lv[MAX_IDX];
int work[MAX_IDX];
int chk[MAX_IDX];

int n;

bool build_level_graph() {
  memset(lv, -1, sizeof lv);
  lv[SRC] = 0;

  queue<int> q;
  q.push(SRC);

  while (!q.empty()) {
    const int cur = q.front();
    q.pop();

    for (int next : graph[cur]) {
      if (lv[next] == -1 && cap[cur][next] - flow[cur][next] > 0) {
        lv[next] = lv[cur] + 1;
        q.push(next);
      }
    }
  }

  return lv[SINK] != -1;
}

int find_blocking_flow(const int cur, const int tot) {
  if (cur == SINK) {
    return tot;
  }

  for (int& i = work[cur]; i < graph[cur].size(); ++i) {
    int next = graph[cur][i];
    if (lv[next] == lv[cur] + 1 && cap[cur][next] - flow[cur][next] > 0) {
      int fl = find_blocking_flow(next, min(tot, cap[cur][next] - flow[cur][next]));
      if (fl > 0) {
        flow[cur][next] += fl;
        flow[next][cur] -= fl;
        return fl;
      }
    }
  }
  // No blocking flow.
  return 0;
}

void find_reachable_people_from(const int from) {
  queue<int> q;
  q.push(from);

  while (!q.empty()) {
    const int cur = q.front();
    q.pop();

    for (int next : graph[cur]) {
      if (chk[next] == 0 && cap[cur][next] - flow[cur][next] > 0) {
        chk[next] = 1;
        q.push(next);
      }
    }
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n;

  for (int i = 1; i <= n; ++i) {
    int team;
    cin >> team;

    if (team == 1) {
      cap[SRC][i] = INF;
      graph[SRC].push_back(i);
      graph[i].push_back(SRC);
    }

    if (team == 2) {
      cap[i][SINK] = INF;
      graph[i].push_back(SINK);
      graph[SINK].push_back(i);
    }
  }

  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= n; ++j) {
      cin >> cap[i][j];
      if (i != j) {
        // Since the given sadness matrix is symmetric, j-to-i path will be
        // established later. Thus, only make push_back once.
        graph[i].push_back(j);
      }
    }
  }

  int ans = 0;
  while (build_level_graph()) {
    memset(work, 0, sizeof work);
    while (true) {
      const int fl = find_blocking_flow(SRC, INF);
      if (fl == 0) {
        break;
      }
      ans += fl;
    }
  }

  cout << ans << "\n";

  // Are reachable people from SRC is in A. Otherwise, in B.
  find_reachable_people_from(SRC);

  vector<int> a, b;
  for (int i = 1; i <= n; ++i) {
    if (chk[i] == 0) {
      b.push_back(i);
    } else {
      a.push_back(i);
    }
  }

  for (int p : a) {
    cout << p << ' ';
  }
  cout << '\n';

  for (int p : b) {
    cout << p << ' ';
  }
  cout << '\n';
}