#include "../../include/component.hpp"
#include "../../include/Subckt.hpp"

void SubcktInstance::set_subckt(std::shared_ptr<Subckt> subckt) {
    subckt_ = subckt;
}

std::shared_ptr<Component> SubcktInstance::clone() const {
    return std::make_shared<SubcktInstance>(*this);
}       