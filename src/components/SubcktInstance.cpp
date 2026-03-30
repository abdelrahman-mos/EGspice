#include "../../include/component.hpp"
#include "../../include/Subckt.hpp"

void SubcktInstance::set_subckt(std::shared_ptr<Subckt> subckt) {
    subckt_ = subckt;
}