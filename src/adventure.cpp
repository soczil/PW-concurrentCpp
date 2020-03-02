#include "adventure.h"

// Packing eggs (used in the best solution) to the bag.
static void eggsToBag(std::vector<Egg>& eggs, BottomlessBag& bag,
                      std::vector<std::vector<uint64_t>>& packed) {
  size_t i = eggs.size() - 1, j = bag.getCapacity();
  uint64_t result = packed[i][j];

  while ((result > 0) && (i >= 0)) {
    try {
      if (packed[i][j] == packed.at(i - 1)[j]) {
        --i;
      } else {
        bag.addEgg(eggs[i]);
        j -= eggs[i].getSize();
        result -= eggs[i].getSize();
        --i;
      }
    } catch (const std::out_of_range& oor) {
      bag.addEgg(eggs[i]);
      result = 0;
    }
  }
}

// Sequential knapsack problem.
uint64_t LonesomeAdventure::packEggs(std::vector<Egg> eggs,
                                     BottomlessBag& bag) {
  if (eggs.size() == 0) {
    return 0;
  }

  uint64_t capacity = bag.getCapacity();
  std::vector<std::vector<uint64_t>> packed(
      eggs.size(), std::vector<uint64_t>(capacity + 1));
  std::vector<std::vector<size_t>> usedEggs(capacity + 1);
  uint64_t result;

  for (size_t i = 0; i < eggs.size(); ++i) {
    for (size_t j = 0; j <= capacity; ++j) {
      if (eggs[i].getSize() > j) {
        try {
          packed[i][j] = packed.at(i - 1)[j];
        } catch (const std::out_of_range& oor) {
          packed[i][j] = 0;
        }
      } else {
        try {
          result =
              eggs[i].getWeight() + packed.at(i - 1).at(j - eggs[i].getSize());
        } catch (const std::out_of_range& oor) {
          result = eggs[i].getWeight();
        }

        try {
          if (result > packed.at(i - 1)[j]) {
            packed[i][j] = result;
          } else {
            packed[i][j] = packed.at(i - 1)[j];
          }
        } catch (const std::out_of_range& oor) {
          packed[i][j] = result;
        }
      }
    }
  }

  eggsToBag(eggs, bag, packed);

  return packed[eggs.size() - 1][capacity];
}

// Assistant function for parallel knapsack problem.
// Computes its part of a row.
static void parallelPacking(std::vector<std::vector<uint64_t>>& packed,
                            Egg& egg, size_t row, size_t l, size_t r) {
  uint64_t result;

  for (size_t i = l; i <= r; ++i) {
    if (egg.getSize() > i) {
      try {
        packed[row][i] = packed.at(row - 1)[i];
      } catch (const std::out_of_range& oor) {
        packed[row][i] = 0;
      }
    } else {
      try {
        result = egg.getWeight() + packed.at(row - 1).at(i - egg.getSize());
      } catch (const std::out_of_range& oor) {
        result = egg.getWeight();
      }

      try {
        if (result > packed.at(row - 1)[i]) {
          packed[row][i] = result;
        } else {
          packed[row][i] = packed.at(row - 1)[i];
        }
      } catch (const std::out_of_range& oor) {
        packed[row][i] = result;
      }
    }
  }
}

// For every row:
// - divides a row into numberOfShamans + 1 pieces;
// - enqueues numberOfShamans tasks;
// - calculates one task by itself;
// - waits for other calculations to finish.
uint64_t TeamAdventure::packEggs(std::vector<Egg> eggs, BottomlessBag& bag) {
  if (eggs.size() == 0) {
    return 0;
  }

  uint64_t capacity = bag.getCapacity();
  size_t size = (capacity + 1) / (numberOfShamans + 1), start;
  std::vector<std::future<void>> futures(numberOfShamans);
  std::vector<std::vector<uint64_t>> packed(
      eggs.size(), std::vector<uint64_t>(capacity + 1));

  for (size_t i = 0; i < eggs.size(); ++i) {
    start = 0;
    if (size != 0) {
      for (size_t j = 0; j < numberOfShamans; ++j) {
        futures[j] = councilOfShamans.enqueue(parallelPacking, std::ref(packed),
                                              std::ref(eggs[i]), i, start,
                                              start + size - 1);
        start += size;
      }
    }

    parallelPacking(packed, eggs[i], i, start, capacity);

    if (size != 0) {
      for (auto& fut : futures) {
        fut.wait();
      }
    }
  }

  eggsToBag(eggs, bag, packed);

  return packed[eggs.size() - 1][capacity];
}

// Merges two sorted parts of an array.
static void merge(std::vector<GrainOfSand>& grains, size_t l, size_t m,
                  size_t r) {
  size_t n1 = m - l + 1, n2 = r - m;
  size_t i, j, k;
  std::vector<GrainOfSand> left(n1);
  std::vector<GrainOfSand> right(n2);

  for (i = 0; i < n1; ++i) {
    left[i] = grains[l + i];
  }

  for (i = 0; i < n2; ++i) {
    right[i] = grains[m + i + 1];
  }

  i = 0;
  j = 0;

  for (k = l; (i < n1) && (j < n2); ++k) {
    if (left[i] < right[j]) {
      grains[k] = left[i];
      ++i;
    } else {
      grains[k] = right[j];
      ++j;
    }
  }

  while (i < n1) {
    grains[k] = left[i];
    ++i;
    ++k;
  }

  while (j < n2) {
    grains[k] = right[j];
    ++j;
    ++k;
  }
}

// MergeSort algorithm for one thread.
static void mergeSort(std::vector<GrainOfSand>& grains, size_t l, size_t r) {
  if (l < r) {
    size_t m = (l + r) / 2;
    mergeSort(grains, l, m);
    mergeSort(grains, m + 1, r);
    merge(grains, l, m, r);
  }
}

// Sequential sand arrangement problem.
void LonesomeAdventure::arrangeSand(std::vector<GrainOfSand>& grains) {
  size_t numberOfGrains = grains.size() - 1;
  mergeSort(grains, 0, numberOfGrains);
}

// MergeSort algorithm for multiple threads.
// Behaves similarly to sequential Merge Sort, but (if it
// is possible) runs sorting of one half in another thread.
static void parallelMergeSort(std::vector<GrainOfSand>& grains, size_t l,
                              size_t r, uint64_t& shamans,
                              std::mutex& shamansProtection,
                              ThreadPool& council) {
  if (l < r) {
    std::future<void> fut;
    size_t m = (l + r) / 2;

    shamansProtection.lock();
    if (shamans > 0) {
      shamans--;
      shamansProtection.unlock();
      fut = council.enqueue(parallelMergeSort, std::ref(grains), l, m,
                            std::ref(shamans), std::ref(shamansProtection),
                            std::ref(council));
    } else {
      shamansProtection.unlock();
      parallelMergeSort(grains, l, m, shamans, shamansProtection, council);
    }

    parallelMergeSort(grains, m + 1, r, shamans, shamansProtection, council);

    if (fut.valid()) {
      fut.wait();
    }

    merge(grains, l, m, r);
  }
}

// Parallel sand arrangement problem.
void TeamAdventure::arrangeSand(std::vector<GrainOfSand>& grains) {
  size_t numberOfGrains = grains.size() - 1;
  std::vector<GrainOfSand> ordered(grains.size());
  std::mutex shamansProtection;
  uint64_t shamans = numberOfShamans;
  parallelMergeSort(grains, 0, numberOfGrains, shamans, shamansProtection,
                    councilOfShamans);
}

// Sequential selection of the best crystal.
Crystal LonesomeAdventure::selectBestCrystal(std::vector<Crystal>& crystals) {
  Crystal max;

  for (auto crystal : crystals) {
    if (max < crystal) {
      max = crystal;
    }
  }

  return max;
}

// Assistant function for parallel selection of the best crystal.
// Selects best crystal in given section.
static Crystal selectMyBest(std::vector<Crystal>& crystals, size_t l,
                            size_t r) {
  Crystal myMax;

  for (size_t i = l; i <= r; ++i) {
    if (myMax < crystals[i]) {
      myMax = crystals[i];
    }
  }

  return myMax;
}

// Parallel selection of the best crystal:
// - divides given vector into numberOfShamans + 1 pieces;
// - enqueues numberOfShamans task;
// - calculates one task by itself;
// - waits for other calculations to finish;
// - chooses max from local maxes.
Crystal TeamAdventure::selectBestCrystal(std::vector<Crystal>& crystals) {
  Crystal max, localMax;
  size_t size = crystals.size() / (numberOfShamans + 1);
  size_t start = 0;
  std::vector<std::future<Crystal>> futures(numberOfShamans);

  if (size != 0) {
    for (size_t i = 0; i < numberOfShamans; ++i) {
      futures[i] = councilOfShamans.enqueue(selectMyBest, std::ref(crystals),
                                            start, start + size - 1);
      start += size;
    }
  }

  max = selectMyBest(crystals, start, crystals.size() - 1);

  if (size != 0) {
    for (auto& fut : futures) {
      localMax = fut.get();
      if (max < localMax) {
        max = localMax;
      }
    }
  }

  return max;
}
