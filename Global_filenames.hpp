#ifndef Global_filenames
#define Global_filenames

/*
Declaration of global variables (works only for C++17 & above)
For all C++ standards, use 'extern' keyword with corresponding cpp file
*/

#include <string>

using namespace std;

inline const std::string FILENAME {"../Expenses.txt"};
inline const std::string BUFFER_FILENAME {"../Temporarystorage.txt"};

#endif

