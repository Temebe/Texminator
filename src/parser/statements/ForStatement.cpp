#include "parser/statements/ForStatement.h"

ForStatement::ForStatement(std::string itName_, std::unique_ptr<Expression> source_, ForType type_,
                           std::unique_ptr<Statement> body_)
                           : iteratorName(std::move(itName_)),
                             source(std::move(source_)),
                             type(type_),
                             body(std::move(body_)) {}

void ForStatement::execute(Environment &environment) {
    environment.createNewScope(local);
    auto stream = std::get<StreamType>(castValue(source->evaluate(environment), STREAM));

    if (type == perCharacter) {
        executePerCharacter(environment, stream);
    } else {
        executePerLine(environment, stream);
    }

    environment.destroyCurrentScope();
}

// TODO Merge these two methods into one
void ForStatement::executePerCharacter(Environment &environment, StreamType& stream_) {
    bool ok;
    CharType iter = stream_->readChar(ok);
    environment.addVariable(iteratorName, castValue(iter, CHAR));

    while (ok) {
        environment.setVariable(iteratorName, castValue(iter, CHAR));
        body->execute(environment);

        if (body->getExecStatus() == broke && body->getExecStatus() == returned) {
            execStatus = body->getExecStatus();
            break;
        }
        iter = stream_->readChar(ok);
    }
}

void ForStatement::executePerLine(Environment &environment, StreamType& stream_) {
    bool ok;
    StringType iter = stream_->readLine(ok);
    environment.addVariable(iteratorName, castValue(iter, STRING));

    while (ok) {
        environment.setVariable(iteratorName, castValue(iter, STRING));
        body->execute(environment);

        if (body->getExecStatus() == broke || body->getExecStatus() == returned) {
            execStatus = body->getExecStatus();
            break;
        }
        iter = stream_->readLine(ok);
    }
}



