#ifndef COMPONENT_HPP
#define COMPONENT_HPP
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <complex>
#include "Matrix.hpp"

class Subckt;

class Component
{
protected:
    std::vector<std::string> terminals;
    std::vector<int> terminals_int;
    std::string name_;
    double value;
    double ac_val;
public:
    Component(std::vector<std::string> terminals, std::string name = "", double value = 0.0) : terminals(terminals), name_(name), value(value), ac_val(value) {}
    Component(std::vector<std::string> terminals, double ac_val, std::string name = "", double value = 0.0) : terminals(terminals), name_(name), value(value), ac_val(ac_val) {}

    virtual void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources, int num_inductors) {
        return;
    }

    virtual void stamp(std::shared_ptr<Matrix<std::complex<double>>> circuit_matrix, std::shared_ptr<Matrix<std::complex<double>>> output_matrix, int num_vsources, double frequency) {
        return;
    }

    virtual std::shared_ptr<Component> clone() const = 0;

    std::string name() const {
        return name_;
    }

    void set_name(std::string new_name) {
        name_ = new_name;
    }

    std::vector<std::string> get_terminals() {
        return this->terminals;
    }

    void set_terminals(std::vector<std::string> new_terminals) {
        this->terminals = new_terminals;
    }

    void update_terminals(std::vector<int> terminals_int) {
        this->terminals_int = terminals_int;
    }
};

class Vsource : public Component {
public:
    int id;
    Vsource(std::vector<std::string> terminals, int vsource_id, std::string name = "", double value = 0.0) : Component(terminals, name, value), id(vsource_id) {}
    Vsource(std::vector<std::string> terminals, int vsource_id, double ac_val, std::string name = "", double value = 0.0) : Component(terminals, ac_val, name, value), id(vsource_id) {}
    void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources, int num_inductors) override;
    void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, double temp_value, int num_vsources, int num_inductors);
    void stamp(std::shared_ptr<Matrix<std::complex<double>>> circuit_matrix, std::shared_ptr<Matrix<std::complex<double>>> output_matrix, int num_vsources, double frequency) override;
    std::shared_ptr<Component> clone() const override;
};

class Isource : public Component
{
public:
    Isource(std::vector<std::string> terminals, std::string name = "", double value = 0.0) : Component(terminals, name, value) {}
    Isource(std::vector<std::string> terminals, double ac_val, std::string name = "", double value = 0.0) : Component(terminals, ac_val, name, value) {}
    void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources, int num_inductors) override;
    void stamp(std::shared_ptr<Matrix<std::complex<double>>> circuit_matrix, std::shared_ptr<Matrix<std::complex<double>>> output_matrix, int num_vsources, double frequency) override;
    std::shared_ptr<Component> clone() const override;
};


class Resistor : public Component {
public:
    Resistor(std::vector<std::string> terminals, std::string name = "", double value = 0.0) : Component(terminals, name, value) {}
    Resistor(std::vector<std::string> terminals, double ac_val, std::string name = "", double value = 0.0) : Component(terminals, ac_val, name, value) {}
    void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources, int num_inductors) override;
    void stamp(std::shared_ptr<Matrix<std::complex<double>>> circuit_matrix, std::shared_ptr<Matrix<std::complex<double>>> output_matrix, int num_vsources, double frequency) override;
    std::shared_ptr<Component> clone() const override;
};

class Inductor : public Component {
public:
    int inductor_id;
    Inductor(std::vector<std::string> terminals, int inductor_id, std::string name = "", double value = 0.0) : Component(terminals, name, value), inductor_id(inductor_id) {}
    void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources, int num_inductors) override;
    void stamp(std::shared_ptr<Matrix<std::complex<double>>> circuit_matrix, std::shared_ptr<Matrix<std::complex<double>>> output_matrix, int num_vsources, double frequency) override;
    std::shared_ptr<Component> clone() const override;
};

class Capacitor : public Component {
public:
    Capacitor(std::vector<std::string> terminals, std::string name = "", double value = 0.0) : Component(terminals, name, value) {}
    void stamp(std::shared_ptr<Matrix<std::complex<double>>> circuit_matrix, std::shared_ptr<Matrix<std::complex<double>>> output_matrix, int num_vsources, double frequency) override;
    std::shared_ptr<Component> clone() const override;
};

class VCCS : public Component {
public:
    VCCS(std::vector<std::string> terminals, std::string name = "", double value = 0.0) : Component(terminals, name, value) {}
    VCCS(std::vector<std::string> terminals, double ac_val, std::string name = "", double value = 0.0) : Component(terminals, ac_val, name, value) {}
    void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources, int num_inductors) override;
    void stamp(std::shared_ptr<Matrix<std::complex<double>>> circuit_matrix, std::shared_ptr<Matrix<std::complex<double>>> output_matrix, int num_vsources, double frequency) override;
    std::shared_ptr<Component> clone() const override;
};

class CCCS : public Vsource {
public:
    CCCS(std::vector<std::string> terminals, int cccs_id, std::string name = "", double value = 0.0) : Vsource(terminals, cccs_id, name, value) {}
    CCCS(std::vector<std::string> terminals, int cccs_id, double ac_val, std::string name = "", double value = 0.0) : Vsource(terminals, cccs_id, ac_val, name, value) {}
    void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources, int num_inductors) override;
    void stamp(std::shared_ptr<Matrix<std::complex<double>>> circuit_matrix, std::shared_ptr<Matrix<std::complex<double>>> output_matrix, int num_vsources, double frequency) override;
    std::shared_ptr<Component> clone() const override;
};

class VCVS : public Vsource {
public:
    VCVS(std::vector<std::string> terminals, int cccs_id, std::string name = "", double value = 0.0) : Vsource(terminals, cccs_id, name, value) {}
    VCVS(std::vector<std::string> terminals, int cccs_id, double ac_val, std::string name = "", double value = 0.0) : Vsource(terminals, cccs_id, ac_val, name, value) {}
    void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources, int num_inductors) override;
    void stamp(std::shared_ptr<Matrix<std::complex<double>>> circuit_matrix, std::shared_ptr<Matrix<std::complex<double>>> output_matrix, int num_vsources, double frequency) override;
    std::shared_ptr<Component> clone() const override;
};

class CCVS : public Vsource {
public:
    CCVS(std::vector<std::string> terminals, int cccs_id, std::string name = "", double value = 0.0) : Vsource(terminals, cccs_id, name, value) {}
    CCVS(std::vector<std::string> terminals, int cccs_id, double ac_val, std::string name = "", double value = 0.0) : Vsource(terminals, cccs_id, ac_val, name, value) {}
    void stamp(std::shared_ptr<Matrix<double>> circuit_matrix, std::shared_ptr<Matrix<double>> output_matrix, int num_vsources, int num_inductors) override;
    void stamp(std::shared_ptr<Matrix<std::complex<double>>> circuit_matrix, std::shared_ptr<Matrix<std::complex<double>>> output_matrix, int num_vsources, double frequency) override;
    std::shared_ptr<Component> clone() const override;
};

class SubcktInstance : public Component {
    std::string subckt_name_;
    std::shared_ptr<Subckt> subckt_;
public:
    SubcktInstance(std::string name = "", std::vector<std::string> terminals = {}, std::string subckt_name = "") : Component(terminals, name, 0.0), subckt_name_(subckt_name) {}
    std::string subckt_name() const {
        return subckt_name_;
    }
    std::shared_ptr<Subckt> parent_subckt() {
        return subckt_;
    }
    void set_subckt(std::shared_ptr<Subckt> subckt);
    std::shared_ptr<Component> clone() const override;
};

#endif