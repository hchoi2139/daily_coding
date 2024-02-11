// https://www.acmicpc.net/problem/14750

#include <algorithm>
#include <cstring>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

using namespace std;

using ll = long long;

constexpr int INF = numeric_limits<int>::max();

constexpr int MAX_NUM_CORNERS = 1000;
constexpr int MAX_CAPACITY_OF_A_HOLE = 5;
constexpr int MAX_NUM_HOLES = 50;
constexpr int MAX_NUM_MICE = 250;

// Index 0 to (MAX_NUM_MICE - 1) are reserved to represent mice.
// Index MAX_NUM_MICE to (MAX_NUM_MICE + MAX_NUM_HOLES - 1) are reserved to
// represent holes.
// Index (MAX_NUM_MICE + MAX_NUM_HOLES) is reserved for SRC.
// Index (MAX_NUM_MICE + MAX_NUM_HOLES + 1) is reserved for SINK.
constexpr int SRC = MAX_NUM_MICE + MAX_NUM_HOLES;
constexpr int SINK = SRC + 1;
constexpr int MICE_HOLE_OFFSET = MAX_NUM_MICE;
constexpr int MAX_IDX = 505;

struct Coord {
  ll x;
  ll y;

  bool operator<(const Coord& other) const {
    if (this->x == other.x) {
      return this->y < other.y;
    }
    return this->x < other.x;
  }

  bool operator<=(const Coord& other) const {
    if (this->x == other.x) {
      return this->y <= other.y;
    }
    return this->x < other.x;
  }

  bool operator==(const Coord& other) const {
    return this->x == other.x && this->y == other.y;
  }
};

vector<int> graph[MAX_IDX];
int flow[MAX_IDX][MAX_IDX];
int capacity[MAX_IDX][MAX_IDX];

Coord corners[MAX_NUM_CORNERS + 1];
Coord holes[MAX_NUM_HOLES];
Coord mice[MAX_NUM_MICE];

int num_corners, capacity_of_a_hole, num_holes, num_mice;

// Returns the sign of the cross product between c1c2 vector and c1c3 vector.
inline int ccw(const Coord& c1, const Coord& c2, const Coord& c3) {
  const ll cp = c1.x * (c2.y - c3.y) +
                c2.x * (c3.y - c1.y) +
                c3.x * (c1.y - c2.y);
  if (cp < 0) { return -1; }
  if (cp > 0) { return 1; }
  return 0;
}

// Returns true if c1c2 line segment intersects with c3c4 line segment.
[[nodiscard]] bool do_intersect(
  const Coord& c1,
  const Coord& c2,
  const Coord& c3,
  const Coord& c4
) {
  const ll c1c2 = ccw(c1, c2, c3) * ccw(c1, c2, c4);
  const ll c3c4 = ccw(c3, c4, c1) * ccw(c3, c4, c2);

  // Case c1c2 and c3c4 lie on a line.
  if (c1c2 == 0 && c3c4 == 0) {
    return min(c1, c2) <= max(c3, c4) &&
           min(c3, c4) <= max(c1, c2);
  }

  return c1c2 <= 0 && c3c4 <= 0;
}

// Finds out the relationship and capacity between mice-hole edges.
void find_holes_each_mouse_can_reach() {
  for (int m = 0; m < num_mice; ++m) {
    for (int h = 0; h < num_holes; ++h) {
      // If m-h line segment and edges of the house intersect once, there is
      // no obstacle between the mice and the hole, thus the hole is reachable.
      // Otherwise, the mice cannot directly look at the hole as some other
      // walls prevent to do so.
      int num_intersections = 0;
      for (int c = 0; c < num_corners; ++c) {
        // In case where the hole is in the corner, we skip once to prevent
        // double counting.
        if (corners[c] == holes[h]) {
          continue;
        }
        // Normal case.
        if (do_intersect(mice[m], holes[h], corners[c], corners[c + 1])) {
          ++num_intersections;
        }
      }

      // In case of one intersection, make an edge from m to h with capacity 1,
      // as mouse cannot clone itself and send its bodies to multiple holes.
      if (num_intersections == 1) {
        int corr_hole = h + MICE_HOLE_OFFSET;
        graph[m].push_back(corr_hole);
        graph[corr_hole].push_back(m);
        capacity[m][corr_hole] = 1;
      }
    }
  }
}

[[nodiscard]] int edmond_karp() {
  int max_flow = 0;

  while (true) {
    int parent[MAX_IDX];
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

  cin >> num_corners >> capacity_of_a_hole >> num_holes >> num_mice;

  for (int c = 0; c < num_corners; ++c) {
    ll x, y;
    cin >> x >> y;
    corners[c] = { x, y };
  }
  corners[num_corners] = corners[0];

  for (int h = 0; h < num_holes; ++h) {
    ll x, y;
    cin >> x >> y;
    holes[h] = { x, y };

    int corr_hole = h + MICE_HOLE_OFFSET;
    graph[corr_hole].push_back(SINK);
    graph[SINK].push_back(corr_hole);
    capacity[corr_hole][SINK] = capacity_of_a_hole;
  }

  for (int m = 0; m < num_mice; ++m) {
    ll x, y;
    cin >> x >> y;
    mice[m] = { x, y };

    graph[SRC].push_back(m);
    graph[m].push_back(SRC);
    capacity[SRC][m] = 1;
  }

  find_holes_each_mouse_can_reach();

  int num_mice_that_can_hide = edmond_karp();

  if (num_mice_that_can_hide == num_mice) {
    cout << "Possible";
  } else {
    cout << "Impossible";
  }

  return 0;
}