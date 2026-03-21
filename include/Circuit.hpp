#ifndef CIRCUIT_HPP
#define CIRCUIT_HPP
#include <vector>
#include <memory>
#include "component.hpp"
#include "Command.hpp"
#include "Matrix.hpp"

class Circuit
{
    std::vector<std::unique_ptr<Component>> components;
    std::vector<std::unique_ptr<Command>> commands;
    std::shared_ptr<Matrix<double>> circuit_matrix;
    std::shared_ptr<Matrix<double>> output_matrix;
    int num_nodes;
    int num_vsources;
public:
    Circuit() = default;

    void add_component(std::unique_ptr<Component> component) {
        num_nodes++;
        if (typeid(component) == typeid(Vsource)) num_vsources++;
        components.push_back(std::move(component));
    }

    void add_command(std::unique_ptr<Command> command) {
        commands.push_back(std::move(command));
    }

    std::shared_ptr<Matrix<double>> get_matrix() {
        return circuit_matrix;
    }

    std::shared_ptr<Matrix<double>> get_output_matrix() {
        return output_matrix;
    }

    void stamp_circuit() {
        if (circuit_matrix == nullptr) {
            circuit_matrix = std::make_shared<Matrix<double>>(num_nodes, num_nodes);
            output_matrix = std::make_shared<Matrix<double>>(num_nodes, 1);
        }
        for (const auto& component : components) {
            component->stamp(circuit_matrix, output_matrix);
        }
    }

    void stamp_circuit(double frequency) {
        if (circuit_matrix == nullptr) {
            circuit_matrix = std::make_shared<Matrix<double>>(num_nodes, num_nodes);
            output_matrix = std::make_shared<Matrix<double>>(num_nodes, 1);
            for (const auto& component : components) {
                component->stamp(circuit_matrix, output_matrix, frequency);
            }
        } else {
            for (const auto& component : components) {
                // in repitition, stamp only devices that will update values in the matrix instead of stamping all devices
                if ((typeid(component) == typeid(Inductor)) || (typeid(component) == typeid(Capacitor))) {
                    component->stamp(circuit_matrix, output_matrix, frequency);
                }
            }
        }
    }
};


#endif