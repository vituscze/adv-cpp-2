// Author: Vít Šefl
// Advanced C++, 2nd assignment
// NPRG051 2012/2013
#ifndef DU_2_RESULT_TYPE_HPP
#define DU_2_RESULT_TYPE_HPP

#include <string>
#include <vector>

// A type of recognized argument.
enum arg_type
{
    POSITIONAL,
    SHORT_NAME,
    LONG_NAME
};

// A struct describing recognized argument.
template <typename CharT>
struct result
{
    typedef std::basic_string<CharT> string_type;

    arg_type                 type; // Type of the argument.
    string_type              name; // Name of the switch, if any.
    std::vector<string_type> rest; // Rest of the parameters, if any.

    result()
        : type(POSITIONAL)
        , name()
        , rest()
    { }
};

#endif // DU_2_RESULT_TYPE_HPP
