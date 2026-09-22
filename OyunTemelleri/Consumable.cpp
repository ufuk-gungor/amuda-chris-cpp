#include "Consumable.h"
#include <print>

Consumable::Consumable(std::string n, double w, double heal) : Item{ n, w }, healAmount{ heal } {}


void Consumable::printDetails() const {
	std::println("{} - {} weight - restores {} health", getName(), getWeight(), healAmount);
}