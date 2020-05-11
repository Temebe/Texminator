#ifndef TEXMINATOR_PARSERTRAITS_H
#define TEXMINATOR_PARSERTRAITS_H

#include "parser/statements/ReturnStatement.h"
#include "parser/statements/BreakStatement.h"
#include "parser/statements/ContinueStatement.h"

template <typename T>
struct isJumpStatement {
    static constexpr bool value {false};
};

template<>
struct isJumpStatement<BreakStatement> {
    static constexpr bool value {true};
};

template<>
struct isJumpStatement<ContinueStatement> {
    static constexpr bool value {true};
};

template<>
struct isJumpStatement<ReturnStatement> {
    static constexpr bool value {true};
};

#endif //TEXMINATOR_PARSERTRAITS_H
