/**
 * @file Source.h
 *
 * @brief Abstract class for reading source
 *
 * Source class is responsible for delivering characters from code. Since code could be either from a standard input
 * or a file (at least for now) Source as a class has to deliver some layer of abstraction which it does with
 * getChar() (reading character) and next() (moving to next character) methods. It also provides an
 */

#ifndef TEXMINATOR_SOURCE_H

#define TEXMINATOR_SOURCE_H


class Source {
public:
    /**
     *
     *
     * @return Current character source is at
     */
    char getChar();
    char peek();
    virtual void goNext() = 0;
    unsigned int getLineNumber() const;
    unsigned int getCharPos() const;

protected:
    void setCharPos(unsigned int charPos);
    void setLineNumber(unsigned int lineNumber);
    void setCurrentChar(char newChar_);
    void setNextChar(char newChar_);

private:
    unsigned int lineNumber = 0;
    unsigned int charPos = 0;
    char currentChar = 0;
    char nextChar = 0;

};


#endif //TEXMINATOR_SOURCE_H
