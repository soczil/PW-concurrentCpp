#ifndef SRC_TYPES_H_
#define SRC_TYPES_H_

#include <vector>

#include "./utils.h"

void burden(uint64_t left, uint64_t right);

class Egg {
 public:
  Egg(uint64_t sizeArg, uint64_t weightArg);

  uint64_t getSize();
  uint64_t getWeight();

 private:
  uint64_t size;
  uint64_t weight;
};

class GrainOfSand {
 public:
  GrainOfSand();

  GrainOfSand(uint64_t sizeArg);  //  NOLINT

  bool operator<(GrainOfSand const& other) const;

  bool operator==(GrainOfSand const& other) const;

  GrainOfSand operator=(GrainOfSand const& other);

 private:
  uint64_t size;
};

class Crystal {
 public:
  Crystal();

  Crystal(uint64_t shininessArg);  // NOLINT

  bool operator<(Crystal const& other) const;

  bool operator==(Crystal const& other);

  Crystal operator=(Crystal const& other);

 private:
  uint64_t shininess;
};

class BottomlessBag {
 public:
  explicit BottomlessBag(uint64_t capacityArg);

  uint64_t getCapacity();

  void addEgg(Egg const& egg);

 private:
  std::vector<Egg> eggs;

  uint64_t capacity;
};

#endif  // SRC_TYPES_H_
