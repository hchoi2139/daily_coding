// https://www.acmicpc.net/problem/1867

#include <cstring>
#include <iostream>
#include <vector>

using namespace std;

constexpr int MAX_N = 500;

vector<int> adj[MAX_N + 1];
int matched_with[MAX_N + 1];
bool visited[MAX_N + 1];

int n, k;

bool dfs(int row) {
  visited[row] = true;

  for (const int& col : adj[row]) {
    int prev_row = matched_with[col];
    if (prev_row == 0 || (!visited[prev_row] && dfs(prev_row))) {
      matched_with[col] = row;
      return true;
    }
  }

  return false;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> k;

  {
    int row, col;
    for (int i = 0; i < k; ++i) {
      cin >> row >> col;
      adj[row].push_back(col);
    }
  }

  int min_vertex_cover = 0;

  for (int row = 1; row <= n; ++row) {
    memset(visited, false, sizeof visited);
    if (dfs(row)) {
      ++min_vertex_cover;
    }
  }

  cout << min_vertex_cover;
}