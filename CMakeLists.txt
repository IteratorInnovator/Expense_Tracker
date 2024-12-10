# Set minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Define the project
project(ExpenseTracker LANGUAGES CXX)
set(PROJECT_DESCRIPTION "A simple expense tracker application")

# Add compile options

add_compile_options(-Wa,-mbig-obj)
add_compile_definitions(CLI11_BOOST_OPTIONAL=0 CLI11_EXPERIMENTAL_OPTIONAL=0 CLI11_STD_OPTIONAL=1)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define source files
set(SOURCE_FILES main.cpp Error.cpp FUNCTIONS.cpp)

# Add executables
add_executable(expense-tracker ${SOURCE_FILES})
target_compile_options(expense-tracker PRIVATE -Wa,-mbig-obj) # placed after add_exe line

# Add CLI11 as a subdirectory
set(ENABLE_TESTING OFF CACHE BOOL "" FORCE) # Disable testing in subdirectories
add_subdirectory(CLI11)

# Link CLI11 to your executable
target_link_libraries(expense-tracker PRIVATE CLI11)

# Include the CLI11 headers
target_include_directories(expense-tracker PRIVATE ${CMAKE_SOURCE_DIR}/CLI11/include)

# Prevent CTest from adding test targets
set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
