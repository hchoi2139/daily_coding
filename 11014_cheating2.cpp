// https://www.acmicpc.net/problem/11014

#include <cstring>
#include <iostream>
#include <vector>

using namespace std;

constexpr int MAX_N = 80;
constexpr int MAX_M = 80;
constexpr int SAFE_N = MAX_N + 2;
constexpr int SAFE_M = MAX_M + 2;

constexpr int num_directions = 6;
constexpr int di[] = { -1, -1, 0, 0, 1, 1 };
constexpr int dj[] = { -1, 1, -1, 1, -1, 1 };

vector<int> adj[SAFE_N * SAFE_M];
int matched_with[SAFE_N * SAFE_M];
bool visited[SAFE_N * SAFE_M];

bool is_sittable[SAFE_N][SAFE_M];

// Notations specified in the problem.
int C, N, M;

inline int to_seat(const int row, const int col) {
  return MAX_M * row + col;
}

bool dfs(const int cur_seat) {
  visited[cur_seat] = true;
  for (const int tar_seat : adj[cur_seat]) {
    int prev_seat = matched_with[tar_seat];
    if (prev_seat == 0 || (!visited[prev_seat] && dfs(prev_seat))) {
      matched_with[tar_seat] = cur_seat;
      return true;
    }
  }
  return false;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> C;

  while (C--) {
    // Initialisation.
    for (int i = 0; i < SAFE_N * SAFE_M; ++i) {
      adj[i].clear();
    }
    memset(matched_with, 0, sizeof matched_with);
    memset(visited, false, sizeof visited);
    memset(is_sittable, false, sizeof is_sittable);

    cin >> N >> M;

    int total_seats = 0;

    char seat_info;
    for (int i = 1; i <= N; ++i) {
      for (int j = 1; j <= M; ++j) {
        cin >> seat_info;
        is_sittable[i][j] = (seat_info == '.');
        total_seats += is_sittable[i][j];
      }
    }

    for (int i = 1; i <= N; ++i) {
      for (int j = 1; j <= M; ++j) {
        // If the current seat is not sittable, disregard.
        if (!is_sittable[i][j]) {
          continue;
        }

        // Make edges that indicate relationship of two-end cheatable seats.
        const int cur_seat = to_seat(i, j);
        for (int k = 0; k < num_directions; ++k) {
          const int ni = i + di[k];
          const int nj = j + dj[k];
          if (!is_sittable[ni][nj]) {
            continue;
          }
          adj[cur_seat].push_back(to_seat(ni, nj));
        }
      }
    }

    // Find number of maximum matchings, which is, by Konig's Thm, number of
    // minimum vertex cover.
    int min_seats_cover = 0;
    for (int i = 1; i <= N; ++i) {
      for (int j = 1; j <= M; j += 2) {
        if (!is_sittable[i][j]) {
          continue;
        }
        memset(visited, false, sizeof visited);
        if (dfs(to_seat(i, j))) {
          ++min_seats_cover;
        }
      }
    }

    cout << total_seats - min_seats_cover << '\n';
  }

  return 0;
}