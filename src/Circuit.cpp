#include "../include/Circuit.hpp"
#include "../include/Subckt.hpp"

Circuit::Circuit()  {
    node_map = {{"0", 0}};
    num_nodes = 0;
    num_inductors = 0;
    num_vsources = 0;
}

std::vector<std::shared_ptr<Subckt>>& Circuit::subckts() {
    return subckts_;
}

void Circuit::add_subckt(std::shared_ptr<Subckt> subckt) {
    subckts_.push_back(std::move(subckt));
}