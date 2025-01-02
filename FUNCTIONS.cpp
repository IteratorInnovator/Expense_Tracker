
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include "Error.hpp"
#include "FUNCTIONS.hpp"
#include <iomanip>
#include "Global_filenames.hpp"

using namespace std;
using std::cout;

ostream &operator<<(ostream &os, const Month &month){
    switch (month){
        case Month::January:
        os << "January";
        break;
        case Month::February:
        os << "February";
        break;
        case Month::March:
        os << "March";
        break;
        case Month::April:
        os << "April";
        break;
        case Month::May:
        os << "May";
        break;
        case Month::June:
        os << "June";
        break; 
        case Month::July:
        os << "July";
        break;
        case Month::August:
        os << "August";
        break;
        case Month::September:
        os << "September";
        break;
        case Month::October:
        os << "October";
        break;
        case Month::November:
        os << "November";
        break;
        case Month::December:
        os << "December";
        break;      
    }
    return os;
}

// helper functions
int get_current_year(){
    time_t tim = time(0);
    tm* year = localtime(&tim);
    return year->tm_year + 1900;
}
string get_current_date(){
    stringstream ss;
    time_t get_time = time(0);
    tm* current_time = localtime(&get_time);
    ss << current_time->tm_year+1900 << "-" << current_time->tm_mon+1 << "-" << current_time->tm_mday;
    return ss.str();
}

int create_new_id(){
    ifstream in_file {FILENAME};
    if (!in_file.is_open()){
        throw UnopenedFileError("File could not be opened.");
    }
    if (is_file_empty(in_file)){
        in_file.close(); 
        return 1; // if file is empty, we return starting id of 1
    }
    string last_line;
    in_file.seekg(-2,ios::end);
    streamoff pos = in_file.tellg();
    while (pos>0){
        pos--;
        in_file.seekg(pos);
        char ch = in_file.get();
        if (ch=='\n'){
            break;
        }
    }
    if (pos==0) in_file.seekg(0,ios::beg);
    getline(in_file,last_line);
    vector<string> words;
    words.reserve(4);
    split(words,last_line,'|');
    return stoi(words.at(0))+1;
}

bool is_file_empty(ifstream &in_file){
    return in_file.peek() == ifstream::traits_type::eof();
    // peek at next file character without extracting it
    // if returned character is EOF (negative integer constant), it means file is empty
}

// command line functions

void add_expense(const string &description, const double &amount){

    if (amount<0) throw InvalidArgumentError("Amount cannot be negative");
    int id = create_new_id();
    string date = get_current_date();
    ofstream out_file {FILENAME,ios::app};
    if (!out_file.is_open()){
        throw UnopenedFileError("File could not be opened for writing.");
    }
    out_file << id << "|" << date << "|" << description << "|" << "$" << amount << '\n';
    out_file.close();
    cout << "Expense added successfully (ID: " << id << ")" << endl;
}

void delete_expense(const int &id){
    if (id<1) throw InvalidArgumentError("ID cannot be 0 or negative");
    bool id_found {false};
    ifstream in_file {FILENAME};
    ofstream buffer_file {BUFFER_FILENAME};
    if (!in_file.is_open() && !buffer_file.is_open()){
        throw UnopenedFileError("Files could not be opened.");
    }
    if (!in_file.is_open()){
        buffer_file.close();
        throw UnopenedFileError("Expense file could not be opened for reading.");
    }
    if (!buffer_file.is_open()){
        in_file.close();
        throw UnopenedFileError("Buffer file could not be opened for writing.");
    }
    if (is_file_empty(in_file)){
        in_file.close();
        buffer_file.close();
        throw EmptyFileError("ID not found as file is empty.");
    }
    string line;
    vector<string> words;
    words.reserve(4);
    while (getline(in_file,line)){
        split(words,line,'|');
        if (stoi(words.at(0))!=id){
            buffer_file << line << '\n';
        }
        else id_found = true; 
        words.clear();
    }
    in_file.close();
    buffer_file.close();
    if (id_found == false) throw InvalidArgumentError("Deletion unsuccessful. ID provided does not exist.");
    ifstream read_buffer_file {BUFFER_FILENAME};
    ofstream rewrite_file {FILENAME};
    if (!read_buffer_file.is_open() && !rewrite_file.is_open()){
        throw UnopenedFileError("Files could not be opened.");
    }
    if (!rewrite_file.is_open()){
        read_buffer_file.close();
        throw UnopenedFileError("Expense file could not be opened for writing.");
    }
    if (!read_buffer_file.is_open()){
        rewrite_file.close();
        throw UnopenedFileError("Buffer file could not be opened for reading.");
    }
    while (getline(read_buffer_file,line)){
        rewrite_file << line << '\n';
    }
    read_buffer_file.close();
    rewrite_file.close();
    cout << "Expense deleted successfully." << endl;
}

void list(){

    ifstream in_file {FILENAME};
    if (!in_file.is_open()){
        throw UnopenedFileError("File could not be opened for reading.");
    }
    if (is_file_empty(in_file)){
        in_file.close();
        throw EmptyFileError("File is empty. Please add a new expense.");
    }
    std::cout.setf(ios::left);
    // print header
    std::cout << setw(4) << "ID";
    std::cout << setw(14) << "Date";
    std::cout << setw(20) << "Description";
    std::cout << "Amount" << endl;
    string line;
    vector<string> splitted_line;
    splitted_line.reserve(4);
    while (getline(in_file,line)){
        split(splitted_line,line,'|');
        std::cout << setw(4) << splitted_line.at(0);
        std::cout << setw(14) << splitted_line.at(1);
        std::cout << setw(20) << splitted_line.at(2);
        std::cout << splitted_line.at(3) << endl;
        splitted_line.clear();
    }
    in_file.close();
    std::cout.unsetf(ios::left);
}

void summary(){
    ifstream in_file {FILENAME};
    if (!in_file.is_open()){
        throw UnopenedFileError("File could not be opened for reading.");
    }
    if (is_file_empty(in_file)){
        in_file.close();
        throw EmptyFileError("File is empty. Please add a new expense.");
    }
    string line;
    double total {0};
    regex amount_pattern("\$(\d+(\.\d+)?)$");
    while (getline(in_file,line)){
        smatch match;
        regex_match(line,match,amount_pattern);
        total += stod(match[1]);
    }
    in_file.close();
    cout << fixed << setprecision(2) << "Total expenses: $" << total << endl;
    cout << setprecision(6);
    cout.unsetf(ios::fixed);
}

void summary(const string &month, const string &year){
    int year_in_numeric = stoi(year);
    if (year_in_numeric<0) throw InvalidArgumentError("Year cannot be negative.");
    int current_year = get_current_year();
    if (year_in_numeric>current_year) throw InvalidArgumentError("Year cannot be in the future");
    int month_in_numeric = stoi(month);
    if (month_in_numeric<1 || month_in_numeric>12) throw InvalidArgumentError("Invalid month entered");
    ifstream in_file {FILENAME};
    if (!in_file.is_open()){
        throw UnopenedFileError("File could not be opened for reading.");
    }
    if (is_file_empty(in_file)){
        in_file.close();
        throw EmptyFileError("File is empty. Please add a new expense.");
    }
    double total {0};
    string target_date = year + "-" + month;
    string line;
    string pattern = "^" + target_date + "-[0][1-9]|[1-2][0-9]|[3][0-1]";
    regex date_pattern(pattern);
    regex amount_pattern("\$(\d+(\.\d+)?)$");
    while (getline(in_file,line)){
        if (regex_match(line,date_pattern)){
            smatch match;
            regex_match(line,match,amount_pattern);
            total += stod(match[1]);
        }
    }
    Month month_in_alpha = static_cast<Month>(month_in_numeric);
    cout << fixed << setprecision(2) <<  "Total expenses in " << month_in_alpha << " " << year << ": $" << total << endl;
    cout << setprecision(6);
    cout.unsetf(ios::fixed);
}

void summary(const int &year){
    if (year<0) throw InvalidArgumentError("Year cannot be negative.");
    int current_year = get_current_year();
    if (year>current_year) throw InvalidArgumentError("Year cannot be in the future");
    ifstream in_file {FILENAME};
    if (!in_file.is_open()){
        throw UnopenedFileError("File could not be opened for reading.");
    }
    if (is_file_empty(in_file)){
        in_file.close();
        throw EmptyFileError("There are currently no recorded expenses. Please add a new expense.");
    }
    const string year_in_alpha = to_string(year);
    double total {0};
    string line;
    string pattern = year_in_alpha + "-";
    regex date_pattern(pattern);
    regex amount_pattern("\$(\d+(\.\d+)?)$");
    while (getline(in_file,line)){
        if (regex_match(line,date_pattern)){
            smatch match;
            regex_match(line,match,amount_pattern);
            total += stod(match[1]);
        }

    }
    cout << fixed << setprecision(2) <<  "Total expenses in " << year << ": $" << total << endl;
    cout << setprecision(6);
    cout.unsetf(ios::fixed);
}

void summary(const string &month){
    int month_in_numeric = stoi(month);
    if (month_in_numeric<1 || month_in_numeric>12) throw InvalidArgumentError("Invalid month entered.");
    ifstream in_file {FILENAME};
    if (!in_file.is_open()){
        throw UnopenedFileError("File could not be opened for reading.");
    }
    if (is_file_empty(in_file)){
        in_file.close();
        throw EmptyFileError("File is empty. Please add a new expense.");
    }
    double total {0};
    int current_year = get_current_year();
    string target_date = to_string(current_year) + "-" + month;
    string line;
    while (getline(in_file,line)){
        vector<string> words;
        words.reserve(4);
        split(words,line,'|');
        string date = words.at(1);
        if (date.find(target_date)!=string::npos){
            vector<string> expense_data;
            expense_data.reserve(2);
            split(expense_data,line,'$');
            total += stod(expense_data.at(1));
        }
    }
    Month month_in_alpha = static_cast<Month>(month_in_numeric);
    cout << fixed << setprecision(2) <<  "Total expenses in this year's " << month_in_alpha << ": $" << total << endl;
    cout << setprecision(6);
    cout.unsetf(ios::fixed);
}

void clear_expenses(){
    ofstream out_file {FILENAME, ios::out | ios::trunc};
    if (!out_file.is_open()) throw UnopenedFileError("Expense file could not be opened.");
    out_file.close();
    cout << "All expenses have been cleared." << endl;
}

void display_menu(){

    std::cout.setf(ios::left);
    // print intro
    std::cout << string(45,'=') << endl;
    std::cout << string(15,' ') << "Expense Tracker" << endl;
    std::cout << string(45,'=') << endl;
    std::cout << "Manage your daily expenses easily using the following commands:\n "<< endl;
    std::cout << "Usage " << endl;
    std::cout << "  expense-tracker <command> --[options]\n" << endl;

    // display list of commands
    std::cout << "Commands: " << endl;
    // add command
    std::cout << "  " << setw(15) << left <<  "add"  << "Add a new expense with description and amount."<< endl; 
    std::cout << string(17,' ') << "Options:" << endl;
    std::cout << string(17,' ') << setw(25) << "--description <text>" << "Description of the expense (required)." << endl;
    std::cout << string(17,' ') << setw(25) << "--amount <value>" << "Amount spent (required).\n" << endl;
    //delete command
    std::cout << "  " << setw(15) << "delete"  << "Delete an expense by its ID."<< endl; 
    std::cout << string(17,' ') << "Options:" << endl;
    std::cout << string(17,' ') << setw(25) << "--id <value>" << "ID of the expense to delete (required).\n" << endl;
    // list command
    std::cout << "  " << setw(15) << "list"  << "List all expenses."<< endl; 
    std::cout << string(17,' ') << "Options:" << endl;
    std::cout << string(17,' ') << "None (Lists all recorded expenses).\n" << endl;
    // summary command
    std::cout << "  " << setw(15) << "summary" << "Show a summary of expenses." << endl;
    std::cout << string(17,' ') << "Options:" << endl;
    std::cout << string(17,' ') << setw(25) << "--month <value>" << "Show expenses for a specific month of the current year.\n" << endl;
    std::cout << string(17,' ') << setw(25) << "--year <value>" << "Show expenses for a specific year.\n" << endl;
    // clear command
    std::cout << "  " << setw(15) << "clear"  << "Clear all expenses."<< endl; 
    std::cout << string(17,' ') << "Options:" << endl;
    std::cout << string(17,' ') << "None (Clears all recorded expenses).\n" << endl;
    // help command
    std::cout << "  " << setw(15) << "help" << "Display this help menu\n" << endl;
    std::cout.unsetf(ios::left);
    //Example command syntax
    std::cout << "Examples:" << endl;
    std::cout << "  Add an expense:" << endl;
    std::cout << "    $ expense-tracker add --description Lunch --amount 20\n" << endl;
    std::cout << "  List all expenses:" << endl;
    std::cout << "    $ expense-tracker list\n" << endl;
    std::cout << "  Delete an expense by its ID:" << endl;
    std::cout << "    $ expense-tracker delete --id 2\n" << endl;
    std::cout << "  View total expenses for August 2024:" << endl;
    std::cout << "    $ expense-tracker summary --year 2024 --month 8\n" << endl;
    // Print contact info
    std::cout << string(64,'=') << endl;
    std::cout << "For further assistance, contact me at harryngkokjing@gmail.com" << endl;
    std::cout << string(64,'=') << endl;
}

