// Author: Vít Šefl
// Advanced C++, 2nd assignment
// NPRG051 2012/2013
#ifndef DU_2_HELP_MESSAGE_HPP
#define DU_2_HELP_MESSAGE_HPP

#include <sstream>
#include <string>

#include "../core/du2action.hpp"
#include "du2constants.hpp"
#include "du2tags.hpp"

namespace details
{
    template <typename CharT, typename Tag>
    struct printer
    {
        typedef details::action_context<CharT>  action_type;
        typedef std::basic_string<CharT>        string_type;
        typedef std::basic_istringstream<CharT> istream_type;
        typedef std::basic_ostringstream<CharT> ostream_type;

        typedef parsing_constants<CharT, Tag>   prefix_constants;
        typedef string_constants<CharT, Tag>    str_constants;
        typedef separator_constants<CharT, Tag> sep_constants;
        typedef numeric_constants<Tag>          num_constants;

        static string_type space(std::size_t n)
        {
            return string_type(n, str_constants::space());
        }

        static string_type create_placeholder(int n)
        {
            ostream_type os;
            os << str_constants::placeholder_attr() << n;

            return os.str();
        }

        static void help_switch_title(ostream_type& os)
        {
            os << str_constants::help_switch_title();
        }

        static void help_switch(ostream_type& os, const action_type& a)
        {
            string_type empty;
            int pos = 0;
            int start = num_constants::indent_arg();
            int limit = num_constants::indent_help();

            bool got_short = a.short_name != empty;
            bool got_long  = a.long_name  != empty;

            string_type short_prefix = prefix_constants::short_prefix();
            string_type long_prefix  = prefix_constants::long_prefix();
            string_type arg_start    = prefix_constants::arg_start();
            string_type arg_sep      = prefix_constants::separator();
            string_type sep          = sep_constants::name_separator();

            int arg_count = a.value.arg_count();

            if (got_short || got_long)
            {
                os << space(start);
                pos += start;
            }

            if (got_short)
            {
                os << short_prefix << a.short_name;
                pos += short_prefix.size() + a.short_name.size();
            }

            if (got_short && got_long)
            {
                os << sep;
                pos += sep.size();
            }

            if (got_long)
            {
                os << long_prefix << a.long_name;
                pos += long_prefix.size() + a.long_name.size();
            }

            if (got_long || got_short)
            {
                bool first = true;
                for (int i = 0; i < arg_count; ++i)
                {
                    if (first)
                    {
                        os << arg_start;
                        pos += arg_start.size();
                        first = false;
                    }
                    else
                    {
                        os << arg_sep;
                        pos += arg_sep.size();
                    }

                    const string_type& p = create_placeholder(i + 1);
                    os << p;
                    pos += p.size();
                }
            }

            if (got_short || got_long)
            {
                if (pos < limit)
                {
                    os << space(limit - pos);
                }
                else
                {
                    os << std::endl << space(limit);
                }

                istream_type is(a.help_message);
                string_type current;

                bool first = true;
                while (std::getline(is, current))
                {
                    if (first)
                    {
                        first = false;
                    }
                    else
                    {
                        os << space(limit);
                    }

                    os << current << std::endl;
                }
            }
        }
    };
}

#endif // DU_2_HELP_MESSAGE_HPP
