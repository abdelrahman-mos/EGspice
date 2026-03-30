#ifndef SUBCKT_HPP
#define SUBCKT_HPP

#include "Circuit.hpp"

class Subckt : public Circuit 
{
    std::string subckt_name;
    std::vector<std::string> terminal_names;
    std::vector<int> terminals_int;

    void update_subckt_terminals() {
        for (auto terminal : terminal_names) {
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
    Subckt(std::string name, std::vector<std::string> terminals) : Circuit(), subckt_name(name), terminal_names(terminals) {
        update_subckt_terminals();
    };

    std::string name() const {
        return subckt_name;
    }
};


#endif /* SUBCKT_HPP */