#ifndef FUNCTIONS
#define FUNCTIONS 
#include "Error.hpp"
#include <vector>
#include <string>
#include <fstream>

using namespace std;

enum class Month {
    January=1,
    February,
    March,
    April,
    May,
    June,
    July,
    August,
    September,
    October,
    November,
    December
};

ostream &operator<<(ostream &os, const Month &month);

//helper functions
int get_current_year();
void split(vector<string> &split_line, const string &line, const char &del); 
string get_current_date();
int create_new_id();
bool is_file_empty(ifstream &in_file);

// command line functions
void add_expense(const string &description, const double &amount); 
void delete_expense(const int &id);
void display_menu(); 
void list(); 
void summary(); 
void summary(const string &month, const string &year);
void summary (const int &year);
void summary (const string &month);
void clear_expenses();

#endif