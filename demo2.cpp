// Author: Vít Šefl
// Advanced C++, 2nd assignment
// NPRG051 2012/2013
#include <ios>
#include <iostream>
#include <string>
#include <vector>

#include "du2all.hpp"
#include "du2lengthof.hpp"

enum prefix_type
{
    KILO,
    MEGA,
    GIGA
};

std::istream& operator>>(std::istream& in, prefix_type& p)
{
    char c;
    in >> c;

    switch (c)
    {
        case 'K': p = KILO; break;
        case 'M': p = MEGA; break;
        case 'G': p = GIGA; break;
        default: in.setstate(std::ios::failbit);
    }

    return in;
}

int main()
{
    parser
        < char
        , DOS
        , unknown_switch_throw_exception
        , argument_leave
        , bad_format_throw_exception
        , unexpected_arg_throw_exception
        , duplicate_throw_exception
        , missing_throw_exception
        > p;

    std::string _args[] =
        {"/P:G"};
    std::vector<std::string> args(_args, _args + lengthof(_args));

    prefix_type prefix;

    p.add_req("P", "", make_get_action<char>(prefix), "Sets the prefix.");

    p.set_args(args.begin(), args.end());

    p.parse();

    switch (prefix)
    {
        case KILO: std::cout << "prefix: KILO"; break;
        case MEGA: std::cout << "prefix: MEGA"; break;
        case GIGA: std::cout << "prefix: GIGA"; break;
    }

    std::cout << "\n" << p.get_help_message();
}
