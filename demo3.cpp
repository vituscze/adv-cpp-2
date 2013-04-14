// Author: Vít Šefl
// Advanced C++, 2nd assignment
// NPRG051 2012/2013
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "du2all.hpp"
#include "du2lengthof.hpp"

int main()
{
    parser
        < char
        , java
        , unknown_switch_try_as_pos_argument
        , argument_delete
        , bad_format_throw_exception
        , unexpected_arg_ignore
        , duplicate_throw_exception
        , missing_throw_exception
        > p;

    std::string _args[] =
        {"-f", "soubor1", "-l", "soubor2", "soubor3"};
    std::vector<std::string> args(_args, _args + lengthof(_args));
    std::vector<std::string> out_args;

    std::string log_file;

    p.add_req("l", "", make_get_action<char>(log_file), "Name of the log file.");

    p.set_args(args.begin(), args.end());

    p.parse();
    p.get_args(std::back_inserter(out_args));

    std::cout << "log file: " << log_file << "\n"
              << "remaining:\n";
    for (const auto& it : out_args)
    {
        std::cout << it << " ";
    }

    std::cout << "\n\n" << p.get_help_message();

}
