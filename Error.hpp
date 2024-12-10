#ifndef ERROR
#define ERROR

#include <string>
#include <stdexcept>

using namespace std;

class EmptyFileError : public exception {
    private:
    string error_msg;
    public:
    EmptyFileError() noexcept = default;
    EmptyFileError(const string &error_msg) noexcept : error_msg {error_msg} {}
    virtual ~EmptyFileError() noexcept = default;
    const char* what() const noexcept override;
};

class InvalidArgumentError : public exception {
    private:
    string error_msg;
    public:
    InvalidArgumentError() noexcept = default;
    InvalidArgumentError(const string &error_msg) noexcept : error_msg {error_msg} {}
    virtual ~InvalidArgumentError() noexcept = default;
    const char* what() const noexcept override;
};

class UnopenedFileError : public exception {
    private:
    string error_msg;
    public:
    UnopenedFileError() noexcept = default;
    UnopenedFileError(const string &error_msg) noexcept : error_msg {error_msg} {}
    virtual ~UnopenedFileError() noexcept = default;
    const char* what() const noexcept override;
};

#endif