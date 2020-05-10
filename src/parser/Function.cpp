#include "parser/Function.h"

// Parameters in function are compared only by their types
bool Function::hasSameParameters(const Function &other_) const {
    return hasSameParameters(other_.parameters);
}

Function::Function(std::unique_ptr<FunctionBody> body_)
    : body(std::move(body_)) {

}

Function::Function(Function &&other) noexcept {
    this->body = std::move(other.body);
    this->parameters = std::move(other.parameters);
}

void Function::addParameter(const Parameter &parameter_) {
    parameters.push_back(parameter_);
}



// TODO std::equal?
bool Function::hasSameParameters(const std::list<Parameter> &other_) const {
    auto aIt = this->parameters.begin();
    auto bIt = other_.begin();
    for (; aIt != parameters.end() && bIt != other_.end(); ++aIt, ++bIt ) {
        if (aIt->second != bIt->second) {
            return false;
        }
    }

    return aIt == parameters.end() && bIt == other_.end();
}

const std::list<Parameter> &Function::getParameters() const {
    return parameters;
}
