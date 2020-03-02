#include "types.h"

void burden(uint64_t left, uint64_t right) {
  volatile uint64_t a = 0;
  for (int i = 0; i < 100; ++i) {
    a += (3 * a - left * 44) + (right / (left == 0 ? 1 : left) + 8);
  }
}

Egg::Egg(uint64_t sizeArg, uint64_t weightArg)
    : size(sizeArg), weight(weightArg) {}

uint64_t Egg::getSize() { return this->size; }
uint64_t Egg::getWeight() {
  burden(this->size, this->weight);
  return this->weight;
}

GrainOfSand::GrainOfSand() : size(0) {}

GrainOfSand::GrainOfSand(uint64_t sizeArg) : size(sizeArg) {}  //  NOLINT

bool GrainOfSand::operator<(GrainOfSand const& other) const {
  burden(this->size, other.size);
  return this->size < other.size;
}

bool GrainOfSand::operator==(GrainOfSand const& other) const {
  return this->size == other.size;
}

GrainOfSand GrainOfSand::operator=(GrainOfSand const& other) {
  this->size = other.size;
  return *this;
}

Crystal::Crystal() : shininess(0) {}

Crystal::Crystal(uint64_t shininessArg) : shininess(shininessArg) {}  // NOLINT

bool Crystal::operator<(Crystal const& other) const {
  burden(this->shininess, other.shininess);
  return this->shininess < other.shininess;
}

bool Crystal::operator==(Crystal const& other) {
  return this->shininess == other.shininess;
}

Crystal Crystal::operator=(Crystal const& other) {
  this->shininess = other.shininess;
  return *this;
}

BottomlessBag::BottomlessBag(uint64_t capacityArg) : capacity(capacityArg) {}

uint64_t BottomlessBag::getCapacity() { return this->capacity; }

void BottomlessBag::addEgg(Egg const& egg) { this->eggs.push_back(egg); }
