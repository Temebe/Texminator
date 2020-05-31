#ifndef TEXMINATOR_STREAM_H
#define TEXMINATOR_STREAM_H

#include <iostream>
#include <fstream>

enum OpenMode {
    readMode, writeMode, readwrite
};

class Stream {
public:
    explicit Stream(const OpenMode mode_) : mode(mode_) {}
    ~Stream();

    virtual void close() = 0;
    virtual char readChar() = 0;
    virtual std::string readLine() = 0;
    virtual void nextChar() = 0;
    virtual void nextLine() = 0;
    virtual void write(const std::string& toWrite_) = 0;
    [[nodiscard]] OpenMode getMode() const;

private:
    const OpenMode mode;
};

class StandardInput : public Stream {
public:
    StandardInput() : Stream(readMode) {}

    void close() override;
    char readChar() override;
    std::string readLine() override;
    void nextChar() override;
    void nextLine() override;
    void write(const std::string& toWrite_) override;
};

class StandardOutput : public Stream {
public:
    StandardOutput() : Stream(writeMode) {}

    void close() override;
    char readChar() override;
    std::string readLine() override;
    void nextChar() override;
    void nextLine() override;
    void write(const std::string& toWrite_) override;
};

class FileStream : public Stream {
public:
    FileStream(const std::string& fileName_, const OpenMode mode_);

    void close() override;
    char readChar() override;
    std::string readLine() override;
    void nextChar() override;
    void nextLine() override;
    void write(const std::string& toWrite_) override;

private:
    void assertGood();
    void assertMode(const OpenMode requiredMode_);

    std::fstream file;
    std::string fileName;

};

#endif //TEXMINATOR_STREAM_H
