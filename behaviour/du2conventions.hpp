// Author: Vít Šefl
// Advanced C++, 2nd assignment
// NPRG051 2012/2013
#ifndef DU_2_CONVENTIONS_HPP
#define DU_2_CONVENTIONS_HPP

#include <string>
#include <vector>

#include "../core/du2resulttype.hpp"
#include "du2constants.hpp"
#include "du2helpmessage.hpp"
#include "du2tags.hpp"

namespace details
{
    template <typename CharT, typename Tag>
    struct helper_parser
    {
        typedef std::basic_string<CharT> string_type;

        static result<CharT> parse_argument(const string_type& s)
        {
            result<CharT> r;
            typename string_type::size_type npos = string_type::npos;
            typename string_type::size_type pos = 0;
            typename string_type::size_type end;

            string_type long_prefix = parsing_constants<CharT, Tag>::long_prefix();
            string_type short_prefix = parsing_constants<CharT, Tag>::short_prefix();
            string_type arg_start = parsing_constants<CharT, Tag>::arg_start();
            string_type separator = parsing_constants<CharT, Tag>::separator();

            if (s.find(long_prefix) == 0)
            {
                r.type = LONG_NAME;
                pos += long_prefix.size();
            }
            else if (s.find(short_prefix) == 0)
            {
                r.type = SHORT_NAME;
                pos += short_prefix.size();
            }
            else
            {
                return r;
            }

            end = s.find(arg_start, pos);
            r.name = s.substr(pos, end - pos);

            if (end == npos)
            {
                return r;
            }

            end += arg_start.size();
            bool finished = false;
            while (!finished)
            {
                pos = end;
                end = s.find(separator, pos);
                r.rest.push_back(s.substr(pos, end - pos));

                if (end == npos)
                {
                    finished = true;
                }

                end += separator.size();
            }

            return r;
        }
    };
}

template <typename CharT>
struct POSIX
{
    typedef std::basic_string<CharT>                    string_type;
    typedef details::printer<CharT, details::POSIX_tag> printer;

    static bool is_length_sensitive()
    {
        return true;
    }

    static bool accept_from_args()
    {
        return true;
    }

    static result<CharT> parse_argument(const string_type& s)
    {
        return details::helper_parser<CharT, details::POSIX_tag>::parse_argument(s);
    }
};

template <typename CharT>
struct DOS
{
    typedef std::basic_string<CharT>                  string_type;
    typedef details::printer<CharT, details::DOS_tag> printer;

    static bool is_length_sensitive()
    {
        return false;
    }

    static bool accept_from_args()
    {
        return false;
    }

    static result<CharT> parse_argument(const string_type& s)
    {
        return details::helper_parser<CharT, details::DOS_tag>::parse_argument(s);
    }
};

template <typename CharT>
struct java
{
    typedef std::basic_string<CharT>                   string_type;
    typedef details::printer<CharT, details::java_tag> printer;

    static bool is_length_sensitive()
    {
        return false;
    }

    static bool accept_from_args()
    {
        return true;
    }

    static result<CharT> parse_argument(const string_type& s)
    {
        return details::helper_parser<CharT, details::java_tag>::parse_argument(s);
    }
};

#endif // DU_2_CONVENTIONS_HPP
