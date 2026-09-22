#include "Item.h"
#include <print>

Item::Item(std::string n, double w) : name{ n }, weight{ w } {} //parametreli constructor


std::string Item::getName() const {
    return name;
}


double Item::getWeight() const {
    return weight;
}


void Item::printDetails() const {
    std::println("{} - {}", name, weight);
}


Item::~Item() {}