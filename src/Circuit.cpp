#include "../include/Circuit.hpp"
#include "../include/Subckt.hpp"

Circuit::Circuit(std::shared_ptr<Logger> logger) {
    node_map = {{"0", 0}};
    num_nodes = 0;
    num_inductors = 0;
    num_vsources = 0;
    curr_node = 1;
    curr_id = 0;
    logger_ = logger;
}

std::vector<std::shared_ptr<Subckt>>& Circuit::subckts() {
    return subckts_;
}

void Circuit::add_subckt(std::shared_ptr<Subckt> subckt) {
    subckts_map_[subckt->name()] = subckt;
    subckts_.push_back(subckt);
}

void Circuit::assign_subckts() {
    for (auto& curr_subckt_instance : subckts_instances_) {
        std::string subckt_name = curr_subckt_instance->subckt_name();
        if (subckts_map_.find(subckt_name) == subckts_map_.end()) {
            logger_->log(LogLevel::ERROR, "Undefined subckt " + subckt_name);
            throw std::runtime_error("Undefined subckt name");
        }
        curr_subckt_instance->set_subckt(subckts_map_[subckt_name]);
    }

    for (auto& curr_pt : subckts_map_) {
        // going a totally flat approach since subckts don't have access to circuit's subckts_map_
        auto curr_subckts_instances = curr_pt.second->subckts_instances();
        for (auto& curr_subckt_instance : curr_subckts_instances) {
            std::string subckt_name = curr_subckt_instance->subckt_name();
            if (subckts_map_.find(subckt_name) == subckts_map_.end()) {
                logger_->log(LogLevel::ERROR, "Undefined subckt " + subckt_name);
                throw std::runtime_error("Undefined subckt name");
            }
            curr_subckt_instance->set_subckt(subckts_map_[subckt_name]);
        }
    }
}

void Circuit::flatten_subckt(std::shared_ptr<SubcktInstance> subckt_instance, const std::string parent_name) {
    std::cout << "subckt name: " << subckt_instance->name() << ", parent name: " << parent_name << std::endl;
    auto subckt = subckt_instance->parent_subckt();
    auto subckts_instances = subckt->subckts_instances();
    std::string new_parent_name = parent_name;
    if (new_parent_name != "") new_parent_name += ".";
    new_parent_name += subckt_instance->name();
    auto flattened_components = subckt->flattened_components(subckt_instance, new_parent_name);
    for (auto& curr_component : flattened_components) {
        if (auto curr_ccvs = std::dynamic_pointer_cast<CCVS>(curr_component)) {
            add_component(curr_ccvs);
        } else if (auto curr_vsource = std::dynamic_pointer_cast<Vsource>(curr_component)) {
            add_component(curr_vsource);
        } else {
            add_component(curr_component);
        }
    }
    for (auto& curr_subckt_instance : subckts_instances) {
        flatten_subckt(curr_subckt_instance, new_parent_name);
    }
}

void Circuit::flatten() {
    logger_->log(LogLevel::INFO, "Flattening Netlist");
    for (auto& curr_subckt_instance : subckts_instances_) {
        std::cout << curr_subckt_instance->name() << " : " << curr_subckt_instance->subckt_name() << std::endl;
        flatten_subckt(curr_subckt_instance, "");
    }
    return;
}