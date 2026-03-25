#ifndef COMPONENT_HPP
#define COMPONENT_HPP
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <complex>
#include "Matrix.hpp"

class Component
{
protected:
    std::vector<std::string> terminals;
    std::vector<int> terminals_int;
    std::string name;
    double value;
    double ac_val;
public:
    Component(std::vector<std::string> terminals, std::string name = "", double value = 0.0) : terminals(terminals), name(name), value(value), ac_val(value) {}
    Component(std::vector<std::string> terminals, double ac_val, std::string name = "", double value = 0.0) : terminals(terminals), name(name), value(value), ac_val(ac_val) {}

    virtual void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources, int num_inductors) {
        return;
    }

    virtual void stamp(std::shared_ptr<Matrix<std::complex<double>>> circuit_matrix, std::shared_ptr<Matrix<std::complex<double>>> output_matrix, int num_vsources, double frequency) {
        return;
    }

    std::vector<std::string> get_terminals() {
        return this->terminals;
    }

    void update_terminals(std::vector<int> terminals_int) {
        this->terminals_int = terminals_int;
    }
};

class Vsource : public Component {
public:
    int vsource_id;
    Vsource(std::vector<std::string> terminals, int vsource_id, std::string name = "", double value = 0.0) : Component(terminals, name, value), vsource_id(vsource_id) {}
    Vsource(std::vector<std::string> terminals, int vsource_id, double ac_val, std::string name = "", double value = 0.0) : Component(terminals, ac_val, name, value), vsource_id(vsource_id) {}
    void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources, int num_inductors) override;
    void stamp(std::shared_ptr<Matrix<std::complex<double>>> circuit_matrix, std::shared_ptr<Matrix<std::complex<double>>> output_matrix, int num_vsources, double frequency) override;
};

class Isource : public Component
{
public:
    Isource(std::vector<std::string> terminals, std::string name = "", double value = 0.0) : Component(terminals, name, value) {}
    Isource(std::vector<std::string> terminals, double ac_val, std::string name = "", double value = 0.0) : Component(terminals, ac_val, name, value) {}
    void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources, int num_inductors) override;
    void stamp(std::shared_ptr<Matrix<std::complex<double>>> circuit_matrix, std::shared_ptr<Matrix<std::complex<double>>> output_matrix, int num_vsources, double frequency) override;
};


class Resistor : public Component {
public:
    Resistor(std::vector<std::string> terminals, std::string name = "", double value = 0.0) : Component(terminals, name, value) {}
    Resistor(std::vector<std::string> terminals, double ac_val, std::string name = "", double value = 0.0) : Component(terminals, ac_val, name, value) {}
    void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources, int num_inductors) override;
    void stamp(std::shared_ptr<Matrix<std::complex<double>>> circuit_matrix, std::shared_ptr<Matrix<std::complex<double>>> output_matrix, int num_vsources, double frequency) override;
};

class Inductor : public Component {
public:
    int inductor_id;
    Inductor(std::vector<std::string> terminals, int inductor_id, std::string name = "", double value = 0.0) : Component(terminals, name, value), inductor_id(inductor_id) {}
    void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources, int num_inductors) override;
    void stamp(std::shared_ptr<Matrix<std::complex<double>>> circuit_matrix, std::shared_ptr<Matrix<std::complex<double>>> output_matrix, int num_vsources, double frequency) override;
};

class Capacitor : public Component {
public:
    Capacitor(std::vector<std::string> terminals, std::string name = "", double value = 0.0) : Component(terminals, name, value) {}
    void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources, int num_inductors) override;
    void stamp(std::shared_ptr<Matrix<std::complex<double>>> circuit_matrix, std::shared_ptr<Matrix<std::complex<double>>> output_matrix, int num_vsources, double frequency) override;
};

#endif