#include "../include/Circuit.hpp"
#include "../include/Subckt.hpp"

Circuit::Circuit()  {
    node_map = {{"0", 0}};
    num_nodes = 0;
    num_inductors = 0;
    num_vsources = 0;
    curr_node = 1;
}

std::vector<std::shared_ptr<Subckt>>& Circuit::subckts() {
    return subckts_;
}

void Circuit::add_subckt(std::shared_ptr<Subckt> subckt) {
    subckts_map_[subckt->name()] = subckt;
    std::cout << "added subckt " << subckt->name() << " to map" << std::endl;
    subckts_.push_back(subckt);
}

void Circuit::flatten() {
    for (auto& curr_subckt_instance : subckts_instances_) {
        std::cout << curr_subckt_instance->name() << " : " << curr_subckt_instance->subckt_name() << std::endl;
        auto;
        // subckt is parsed with 
    }
    return;
}