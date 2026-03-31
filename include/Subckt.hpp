#ifndef SUBCKT_HPP
#define SUBCKT_HPP

#include "Circuit.hpp"

class Subckt : public Circuit 
{
    std::string subckt_name;
    std::vector<std::string> terminal_names_;
    std::vector<int> terminals_int;

    void update_subckt_terminals() {
        for (auto terminal : terminal_names_) {
            auto curr_terminal = node_map.find(terminal);
            if (curr_terminal == node_map.end()) {
                terminals_int.push_back(curr_node);
                node_map.insert({terminal, curr_node++});
            } else {
                terminals_int.push_back(curr_terminal->second);
            }
        }
        num_nodes = curr_node-1;
    }
public:
    Subckt(std::string name, std::vector<std::string> terminals, std::shared_ptr<Logger> logger) : Circuit(logger), subckt_name(name), terminal_names_(terminals) {
        update_subckt_terminals();
    };

    std::string name() const {
        return subckt_name;
    }

    std::vector<std::string> terminal_names() const {
        return terminal_names_;
    }

    std::vector<std::string> flatten_component_nodes(std::shared_ptr<SubcktInstance> subckt_instance, std::string parent_name, std::shared_ptr<Component> curr_component) {
        auto curr_component_terminals = curr_component->get_terminals();
        auto curr_instance_terminals = subckt_instance->get_terminals();
        auto parent_subckt = subckt_instance->parent_subckt();
        auto subckt_terminals = parent_subckt->terminal_names();
        std::vector<std::string> flattened_nodes;
        for (auto& curr_component_terminal : curr_component_terminals) {
            bool hierarchical_node = false;
            if (curr_component_terminal == "0") {
                flattened_nodes.push_back("0");
                continue;
            }
            for (size_t i = 0; i < subckt_terminals.size(); i++) {
                if (curr_component_terminal == subckt_terminals[i]) {
                    flattened_nodes.push_back(curr_instance_terminals[i]);
                    hierarchical_node = true;
                    break;
                }
            }
            if (!hierarchical_node) {
                flattened_nodes.push_back(parent_name + "." + curr_component_terminal);
            }
        }
        return flattened_nodes;
    }

    std::vector<std::shared_ptr<Component>> flattened_components(std::shared_ptr<SubcktInstance> subckt_instance, std::string parent_name) {
        std::vector<std::shared_ptr<Component>> output;
        for (auto& curr_subckt_instance : subckts_instances_) {
            auto flattened_nodes = flatten_component_nodes(subckt_instance, parent_name, curr_subckt_instance);
            curr_subckt_instance->set_terminals(flattened_nodes);
        }
        for (auto& curr_component : components_) {
            auto new_component = curr_component->clone();
            std::string new_name = parent_name + "." + new_component->name();
            auto flattened_nodes = flatten_component_nodes(subckt_instance, parent_name, curr_component);
            new_component->set_name(new_name);
            new_component->set_terminals(flattened_nodes);
            output.push_back(new_component);
        }
        return output;
    } 
};


#endif /* SUBCKT_HPP */