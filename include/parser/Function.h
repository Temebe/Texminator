#ifndef TEXMINATOR_FUNCTION_H
#define TEXMINATOR_FUNCTION_H

#include <memory>
#include <list>

#include "FunctionBody.h"
#include "Value.h"

// Name of the parameter and it's type
using Parameter = std::pair<std::string, ValueEnum>;

class Function {
public:
    explicit Function(std::unique_ptr<FunctionBody> body_);
    Function(Function &&other) noexcept;
    [[nodiscard]] bool hasSameParameters(const Function &other_) const;
    [[nodiscard]] bool hasSameParameters(const std::list<Parameter> &other_) const;
    void addParameter(const Parameter& parameter_);
    [[nodiscard]] const std::list<Parameter>& getParameters() const;

private:
    std::unique_ptr<FunctionBody> body;
    std::list<Parameter> parameters;
    Parameter returnType;
};


#endif //TEXMINATOR_FUNCTION_H
