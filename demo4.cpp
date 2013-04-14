// Author: Vít Šefl
// Advanced C++, 2nd assignment
// NPRG051 2012/2013
#include <iostream>
#include <string>
#include <vector>

#include "du2all.hpp"
#include "du2lengthof.hpp"

int main()
{
    parser
        < char
        , POSIX
        , unknown_switch_try_as_pos_argument
        , argument_leave
        , bad_format_throw_exception
        , unexpected_arg_throw_exception
        , duplicate_throw_exception
        , missing_throw_exception
        > p;

    std::string _args[] =
        {"-20", "42"};
    std::vector<std::string> args(_args, _args + lengthof(_args));

    int start;
    int end;
    int step = 1;

    p.add_pos_req(make_get_action<char>(start));
    p.add_pos_req(make_get_action<char>(end));
    p.add_pos_opt(make_get_action<char>(step));

    p.set_args(args.begin(), args.end());

    p.parse();

    std::cout << "start: " << start << "\n"
              << "end:   " << end   << "\n"
              << "step:  " << step  << "\n";
}
