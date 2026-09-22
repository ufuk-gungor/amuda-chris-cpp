#include "Location.h"
#include <print>


Location::Location(std::string n, std::string d) : name{ n }, description{ d }, north{ nullptr }, south{ nullptr }, west{ nullptr }, east{ nullptr }, isLocked { false }, key{ "" } {}


void Location::printLocationInfo() const {
	std::println("Name: {}", name);
	std::println("Description: {}", description);
}


void Location::setConnections(Location* n, Location* s, Location* e, Location* w) {
	north = n;
	south = s;
	east = e;
	west = w;
}


void Location::setKey(std::string keyName) {
	isLocked = true;
	key = keyName;
}