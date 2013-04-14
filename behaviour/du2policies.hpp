// Author: Vít Šefl
// Advanced C++, 2nd assignment
// NPRG051 2012/2013
#ifndef DU_2_POLITICS_HPP
#define DU_2_POLITICS_HPP

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <cstdlib>

#include "du2conventions.hpp"

struct default_throw_exception
{
    static void handle_error()
    {
        throw std::runtime_error("Parser exception.");
    }
};

struct default_exit
{
    static void handle_error()
    {
        std::exit(1);
    }
};

struct default_print_and_exit
{
    static void handle_error()
    {
        std::cerr << "Error parsing arguments.";
        std::exit(1);
    }
};

struct default_wprint_and_exit
{
    static void handle_error()
    {
        std::wcerr << L"Error parsing arguments.";
        std::exit(1);
    }
};

struct default_ignore
{
    static void handle_error()
    { }
};

typedef default_throw_exception bad_format_throw_exception;
typedef default_exit            bad_format_exit;
typedef default_print_and_exit  bad_format_print_and_exit;
typedef default_wprint_and_exit bad_format_wprint_and_exit;

typedef default_throw_exception unexpected_arg_throw_exception;
typedef default_exit            unexpected_arg_exit;
typedef default_print_and_exit  unexpected_arg_print_and_exit;
typedef default_wprint_and_exit unexpected_arg_wprint_and_exit;
typedef default_ignore          unexpected_arg_ignore;

typedef default_throw_exception duplicate_throw_exception;
typedef default_exit            duplicate_exit;
typedef default_print_and_exit  duplicate_print_and_exit;
typedef default_wprint_and_exit duplicate_wprint_and_exit;
typedef default_ignore          duplicate_redo;

typedef default_throw_exception missing_throw_exception;
typedef default_exit            missing_exit;
typedef default_print_and_exit  missing_print_and_exit;
typedef default_wprint_and_exit missing_wprint_and_exit;
typedef default_ignore          missing_ignore;

// A const iterator would be more appropriate in the following structs, however,
// latest gcc's standard library doesn't know that C++11 specifies
// std::vector<T>::erase to take two const iterators.
template <typename CharT>
struct argument_leave
{
    typedef std::basic_string<CharT>       string_type;
    typedef std::vector<string_type>       vector_type;
    typedef typename vector_type::iterator iterator;

    static iterator update(vector_type&, iterator, iterator end)
    {
        return end;
    }
};

template <typename CharT>
struct argument_delete
{
    typedef std::basic_string<CharT>       string_type;
    typedef std::vector<string_type>       vector_type;
    typedef typename vector_type::iterator iterator;

    static iterator update(vector_type& v, iterator begin, iterator end)
    {
        return v.erase(begin, end);
    }
};

struct unknown_switch_throw_exception
{
    static bool handle_error()
    {
        throw std::runtime_error("Parser exception.");
    }
};

struct unknown_switch_exit
{
    static bool handle_error()
    {
        std::exit(1);
    }
};

struct unknown_switch_throw_print_and_exit
{
    static bool handle_error()
    {
        std::cerr << "Error parsing arguments.";
        std::exit(1);
    }
};

struct unknown_switch_throw_wprint_and_exit
{
    static bool handle_error()
    {
        std::wcerr << L"Error parsing arguments.";
        std::exit(1);
    }
};

struct unknown_switch_try_as_pos_argument
{
    static bool handle_error()
    {
        return true;
    }
};

#endif // DU_2_POLITICS_HPP
