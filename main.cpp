#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <optional>
#include <vector>
#include "Global_filenames.hpp"
#include "FUNCTIONS.hpp"
#include "Error.hpp"
#include "CLI\CLI.hpp"


using namespace std;


/*
argc: argument count
argument count is always at least 1, which is the program's name
argv: argument vector (array of c-style strings, representing CLI arguments)
argv[0]: program name
argv[1]-argv[argc-1]: additional arguments passed into the program
*/
int main(int argc, char* argv[]) {
    CLI::App app {"expense-tracker"};
    app.require_subcommand(0,1); // set min and max no. of subcommands required
    if (argc<2) {
        cerr << "No arguments entered. Enter 'help' for list of commands." << endl;
    }
// add expense
    string description;
    double amount;
    auto add_command = app.add_subcommand("add")->ignore_case();
    add_command->add_option("--description",description)->required();
    add_command->add_option("--amount",amount)->required();
    add_command->callback([&](){
        try { add_expense(description,amount); }
        catch (InvalidArgumentError &error){ cerr << "Error: " << error.what() << endl; }
        catch (UnopenedFileError &error){ cerr << "Error: " << error.what() << endl; }
        catch (logic_error &error) { cerr << "Error: " << error.what() << endl; }
        catch (...) { cerr << "Unknown error encountered." << endl; }
    });

// delete expense
    int id;
    auto delete_command = app.add_subcommand("delete")->ignore_case();
    delete_command->add_option("--id",id)->required()->expected(1);
    delete_command->callback([&](){
        try { delete_expense(id); }
        catch (UnopenedFileError &error){ cerr << "Error: " << error.what() << endl; }
        catch (EmptyFileError &error){ cerr << "Error: " << error.what() << endl; }
        catch (InvalidArgumentError &error){ cerr << "Error: " << error.what() << endl; }
        catch (...) { cerr << "Unknown error encountered." << endl; }
    });

// list all expenses
    auto list_command = app.add_subcommand("list")->ignore_case();
    list_command->callback([](){
        try { list(); }
        catch (UnopenedFileError &error){ cerr << "Error: " << error.what() << endl; }
        catch (EmptyFileError &error){ cerr << "Error: " << error.what() << endl; }
        catch (...) { cerr << "Unknown error encountered." << endl; }
    });

// summary of expenese;
    int current_year = get_current_year();
    optional<int> year {nullopt}, month {nullopt};
    auto summary_command = app.add_subcommand("summary")->ignore_case();
    summary_command->add_option("--year",year)->expected(1);
    summary_command->add_option("--month",month)->expected(1); // set expected number of argument
    summary_command->callback([&](){
        try {
            if (!month && !year){
                summary();
            } else if (month && !year){
                string mth = to_string(month.value());
                summary(mth);
            } else if (!month && year){
                int y = year.value();
                summary(y);
            } else if (month && year){
                string mth = to_string(month.value());
                string y = to_string(year.value());
                summary(mth,y);
            }
        }
        catch (InvalidArgumentError &error) { cerr << "Error: " << error.what() << endl; }
        catch (UnopenedFileError &error){ cerr << "Error: " << error.what() << endl; }
        catch (EmptyFileError &error){ cerr << "Error: " << error.what() << endl; }
        catch (...) { cerr << "Unknown error encountered." << endl; }
    });
// clear expenses
    auto clear_command = app.add_subcommand("clear")->ignore_case();
    clear_command->callback([](){
        try { clear_expenses(); }
        catch (UnopenedFileError &error) { cerr << error.what() << endl; }
        catch (...) { cerr << "Unknown error encountered." << endl; }
    });
// display help menu
    auto help_command = app.add_subcommand("help")->ignore_case();
    help_command->callback([](){
        display_menu();
    });

    try { (app).parse(argc,argv); } 
    catch(const CLI::ParseError &e) { return (app).exit(e); };
    
    return 0;
}



