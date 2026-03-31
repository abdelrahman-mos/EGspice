#include "../include/Circuit.hpp"
#include "../include/Subckt.hpp"

Circuit::Circuit(std::shared_ptr<Logger> logger) {
    node_map = {{"0", 0}};
    num_nodes = 0;
    num_inductors = 0;
    num_vsources = 0;
    curr_node = 1;
    logger_ = logger;
}

std::vector<std::shared_ptr<Subckt>>& Circuit::subckts() {
    return subckts_;
}

void Circuit::add_subckt(std::shared_ptr<Subckt> subckt) {
    subckts_map_[subckt->name()] = subckt;
    std::cout << "added subckt " << subckt->name() << " to map" << std::endl;
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
        add_component(curr_component);
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
        // We need to find a way to flatten the circuit, subckts can also contain other subckts, either we stop assigning node numbers at element parsing,
        // we can then start appending each subckt's inner components with their parents name, then go through each element and encode its terminals,
        // or we find a way to include the previously encoded terminals with the encoded ones in the subckt.
        // we also need to find a way to map the instance terminals to the inner subckt terminals, for example:
        /*
        .subckt my_subckt in out gnd
        r1 in out 1k
        c1 out gnd 1u
        .ends my_subckt
        x1 my_input my_output 0
        */
       //the previous subckt instance should be replaced with the following:
       /*
       x1.r1 my_input my_output 1k
       x1.c1 my_output 0 1u
       */
      // If we have any internal nodes they should be replaced by <subckt_instance_name>.<internal_node_name>. Which means that we won't replace
      // internal nodes
    }
    return;
}