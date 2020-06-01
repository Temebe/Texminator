#ifndef TEXMINATOR_FUNCTION_H
#define TEXMINATOR_FUNCTION_H

#include <memory>
#include <list>

#include "Value.h"
class Statement;

// Name of the parameter and it's type
using Parameter = std::pair<std::string, ValueEnum>;

class Function {
public:
    explicit Function(std::unique_ptr<Statement> body_);
    Function(Function &&other) noexcept;
    ~Function();
    [[nodiscard]] bool hasSameParameters(const Function &other_) const;
    [[nodiscard]] bool hasSameParameters(const std::list<Parameter> &other_) const;
    void addParameter(const Parameter& parameter_);
    [[nodiscard]] const std::list<Parameter>& getParameters() const;

private:
    std::unique_ptr<Statement> body;
    std::list<Parameter> parameters;
    Parameter returnType;
};


#endif //TEXMINATOR_FUNCTION_H
