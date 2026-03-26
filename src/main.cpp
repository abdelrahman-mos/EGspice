#include "../include/Simulator.hpp"

int main(int argc, char* argv[]) {
    Simulator simulator = Simulator(argc, argv);
    simulator.run();
}