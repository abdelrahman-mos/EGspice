#ifndef CIRCUIT_HPP
#define CIRCUIT_HPP
#include <vector>
#include <memory>
#include "component.hpp"
#include "Matrix.hpp"

class Circuit
{
    std::vector<std::unique_ptr<Component>> components;
    int num_nodes;
    std::shared_ptr<Matrix<double>> circuit_matrix;
public:
    Circuit() = default;

    void add_component(std::unique_ptr<Component> component) {
        num_nodes++;
        components.push_back(std::move(component));
    }

    std::shared_ptr<Matrix<double>> get_matrix() {
        return circuit_matrix;
    }

    void stamp_circuit() {
        circuit_matrix = std::make_shared<Matrix<double>>(num_nodes, num_nodes);
        for (const auto& component : components) {
            component->stamp(circuit_matrix);
        }
    }
};


#endif