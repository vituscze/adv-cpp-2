// Author: Vít Šefl
// Advanced C++, 2nd assignment
// NPRG051 2012/2013
#include <exception>
#include <iostream>

#include "du2all.hpp"

int main(int argc, char** argv)
{
    parser
        < char                            // Underlying character type.
        , POSIX                           // Convention type.
        , unknown_switch_throw_exception  // How to handle unknown switch.
        , argument_leave                  // How to handle recognized arguments.
        , bad_format_throw_exception      // How to handle badly formatted arguments.
        , unexpected_arg_throw_exception  // How to handle unexpected arguments.
        , duplicate_throw_exception       // How to handle duplicate arguments.
        , missing_throw_exception         // How to handle missing arguments.
        > p;

    double angle;

    // Adds a required positional argument and an action that parses it and
    // stores the result into angle.

    // The template argument specifies the underlying character type, second
    // template argument is infered from function argument.
    p.add_pos_req(make_get_action<char>(angle));

    int count = 1;

    // Adds an optional positional argument and stores the result into count.
    p.add_pos_opt(make_get_action<char>(count));

    bool verbose = false;
    bool help = false;

    // Adds an optional switch, recognized under the names "v" and "verbose",
    // first name specified a short name, the second one specified a long name.
    // Upon recognizing a verbose switch, store true into verbose. Last argument
    // specifies a help message.
    p.add_opt("v", "verbose", make_set_action<char>(verbose, true)
        , "Produce additional debug output or something.\nMight do something cool.");

    p.add_opt("h", "help", make_set_action<char>(help, true)
        , "Show this message.");

    std::string filename;

    // Adds a required switch, again "o" being the short name and "output" the
    // long name. This argument has to be present in argv, otherwise this
    // exceptional state is handled using the Missing policy.
    p.add_req("o", "output", make_get_action<char>(filename)
        , "Specifies where to store the result.");

    // Sets the arguments. Note that this function operates with a pair of
    // iterators and copies the data into its internal storage. For internal
    // consistency and simplicity, parser attempts to parse every argument
    // in its internal storage. Therefore we explicitly ignore program name.
    p.set_args(argv + 1, argv + argc);

    try
    {
        p.parse();
    }
    catch (std::exception&)
    {
        help = true;
    }

    if (help)
    {
        std::cout << "Usage: prog num1 num2 -o name\n\n";
        std::cout << p.get_help_message() << "\n";

        return 1;
    }

    // This function copies the internal storage via output iterator. Useful
    // when Argument policy specified to delete recognized arguments.
    // p.get_args(output_iterator)

    // Resets all positional arguments and switches into unused state, useful
    // if we wish to correctly handle duplicate arguments and reuse the parser.
    p.reset();

    // Restores parser into its default state. Deletes all rules and action
    // bindings.
    p.clear();

    std::cout << "angle:   " << angle << "\n"
              << "count:   " << count << "\n"
              << "output:  " << filename << "\n"
              << "verbose: " << std::boolalpha << verbose << "\n";

    // See demo1.cpp through demo4.cpp to see additional examples of usage
    // (not commented).
}
