#pragma once
#include <string>

class Item {          // envanterdeki objeler için ana sınıf
public:
    Item(std::string, double);
    std::string getName() const;
    double getWeight() const;
    virtual void printDetails() const;
    virtual ~Item();   // Item pointerı new ile yaratılan bir alt sınıf objesini işaret ederken destructorını çağırmaya çalıştığında sadece kendininkini çağırmaması için virtual destructor

private:
    std::string name;
    double weight;

};