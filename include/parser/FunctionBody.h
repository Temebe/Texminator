#ifndef TEXMINATOR_FUNCTIONBODY_H
#define TEXMINATOR_FUNCTIONBODY_H

/**
 * FunctionBody is a solution to problem in keeping functions in environment.
 * Function is just a body with parameters. This body is a statement which executes in environment. However
 * function is kept in scope which is a part of the environment. This creates an include loop and best solution
 * I could think of at the moment is creating a tag by which body will be kept. It'll then be casted to a statement
 * and executed.
 */
struct FunctionBody {};

#endif //TEXMINATOR_FUNCTIONBODY_H
