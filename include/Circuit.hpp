#ifndef CIRCUIT_HPP
#define CIRCUIT_HPP
#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>
#include <complex>
#include "component.hpp"
#include "Command.hpp"
#include "Matrix.hpp"

class Subckt;

class Circuit
{
    std::unordered_map<std::string, int> node_map;
    std::vector<std::unique_ptr<Command>> commands_;
    std::vector<std::shared_ptr<Subckt>> subckts_;

    void get_and_update_terminals(std::shared_ptr<Component> component) {
        static int curr_node = 1;
        std::vector<std::string> terminals = component->get_terminals();
        std::vector<int> terminals_int;
        for (auto terminal : terminals) {
            auto curr_terminal = node_map.find(terminal);
            if (curr_terminal == node_map.end()) {
                terminals_int.push_back(curr_node);
                node_map.insert({terminal, curr_node++});
            } else {
                terminals_int.push_back(curr_terminal->second);
            }
        }
        component->update_terminals(terminals_int);
        num_nodes = curr_node-1;
    }

protected:
    std::vector<std::shared_ptr<Component>> components_;
    std::vector<std::shared_ptr<Vsource>> vsources_;
    std::vector<std::shared_ptr<Inductor>> inductors_;
    std::vector<std::shared_ptr<SubcktInstance>> subckts_instances_;
    size_t num_nodes;
    size_t num_vsources;
    size_t num_inductors;
public:
    Circuit();

    int numNodes() const {
        return num_nodes;
    }

    int numVsources() const {
        return num_vsources;
    }

    int numInductors() const {
        return num_inductors;
    }

    std::unordered_map<std::string, int> nodeMap() const {
        return node_map;
    }

    std::vector<std::shared_ptr<Component>>& components() {
        return components_;
    }

    std::vector<std::shared_ptr<Vsource>>& vsources() {
        return vsources_;
    }

    std::vector<std::shared_ptr<Inductor>>& inductors() {
        return inductors_;
    }

    std::vector<std::shared_ptr<SubcktInstance>>& subckts_instances() {
        return subckts_instances_;
    }

    std::vector<std::unique_ptr<Command>>& commands() {
        return commands_;
    }

    std::vector<std::shared_ptr<Subckt>>& subckts();

    void add_component(std::shared_ptr<Component> component) {
        get_and_update_terminals(component);
        components_.push_back(component);
    }

    void add_component(std::shared_ptr<Vsource> component) {
        num_vsources++;
        get_and_update_terminals(component);
        vsources_.push_back(component);
        components_.push_back(component);
    }

    void add_component(std::shared_ptr<CCVS> component) {
        num_vsources += 2;
        get_and_update_terminals(component);
        vsources_.push_back(component);
        components_.push_back(component);
    }

    void add_component(std::shared_ptr<Inductor> component) {
        num_inductors++;
        get_and_update_terminals(component);
        inductors_.push_back(component);
        components_.push_back(component);
    }

    void add_component(std::shared_ptr<SubcktInstance> subckt) {
        get_and_update_terminals(subckt);
        subckts_instances_.push_back(subckt);
    }

    void add_command(std::unique_ptr<Command> command) {
        commands_.push_back(std::move(command));
    }

    void add_subckt(std::shared_ptr<Subckt> subckt);

    void flatten() {
        return;
    }
};

#endif