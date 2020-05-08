#ifndef TEXMINATOR_SCOPE_H
#define TEXMINATOR_SCOPE_H

#include <unordered_map>

#include "Value.h"

enum ScopeType {
        local, function
};


class Scope {
public:
    ScopeType getType();
    std::optional<Value> getVariable(const std::string &name_);
    bool containsVariable(const std::string &name_);

private:
    std::unordered_map<std::string, Value> variables;
    ScopeType type;

};


#endif //TEXMINATOR_SCOPE_H
