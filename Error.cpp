#include "Error.hpp"
#include <string>
#include <stdexcept>

using namespace std;

// override only needs to be declared in method declaration
// no need to include again in definition

const char* UnopenedFileError::what() const noexcept{
    return error_msg.c_str();
}

const char* InvalidArgumentError::what() const noexcept{
    return error_msg.c_str();
}

const char* EmptyFileError::what() const noexcept{
    return error_msg.c_str();
}

