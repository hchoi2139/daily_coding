#include <cstring>
#include <iostream>

namespace shark {
  // Upper-limit of the number of sharks.
  constexpr int kMaxSharks = 50;

  struct Shark {
    int size;
    int speed;
    int iq;

    bool operator<(const Shark& other) const {
      return !(this->size > other.size ||
               this->speed > other.speed ||
               this->iq > other.iq);
    }

    bool operator==(const Shark& other) const {
      return this->size == other.size &&
             this->speed == other.speed &&
             this->iq == other.iq;
    }
  };
}

shark::Shark sharks[shark::kMaxSharks];

// eaten_by[i] indicates that sharks[i] was eaten by sharks[eaten_by[i]].
int eaten_by[shark::kMaxSharks];

// visited[i] is true indicates that the algorithm had investigated during
// its earlier dfs that sharks[i], as a predator, was matched to some prey(s).
bool visited[shark::kMaxSharks];

// Actual number of sharks given by the question.
int num_sharks;

bool dfs(int predator) {
  // Indicates that the matching of predator has been attempted.
  visited[predator] = true;

  for (int i = 0; i < num_sharks; ++i) {
    // When two sharks are equal, only allow the larger indexed shark to eat
    // the lower indexed shark.
    if (sharks[predator] == sharks[i] && predator < i) {
      continue;
    }
    // When predator is smaller than the target prey or is itself, disregard.
    if (!(sharks[i] < sharks[predator]) || predator == i) {
      continue;
    }
    // If the target prey was not eaten yet, predator eats it.
    //
    // If the target prey was eaten by other but the original predator can
    // manage to find other prey, predator eats it and eaten_by relationship
    // gets modified in its cascading dfs.
    if (eaten_by[i] == -1 || (!visited[eaten_by[i]] && dfs(eaten_by[i]))) {
      eaten_by[i] = predator;
      return true;
    }
  }
  return false;
}

int main(int argc, char* argv[]) {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> num_sharks;

  int size, speed, iq;
  for (int i = 0; i < num_sharks; ++i) {
    std::cin >> size >> speed >> iq;
    sharks[i].size = size;
    sharks[i].speed = speed;
    sharks[i].iq = iq;
  }

  memset(eaten_by, -1, sizeof eaten_by);

  // Number of successful matches, i.e. successful matches.
  int num_preys = 0;

  for (int i = 0; i < num_sharks; ++i) {
    for (int j = 0; j < 2; ++j) {
      memset(visited, false, sizeof visited);
      if (dfs(i)) {
        ++num_preys;
      }
    }
  }

  std::cout << num_sharks - num_preys;
}