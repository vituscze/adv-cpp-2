// Author: Vít Šefl
// Advanced C++, 2nd assignment
// NPRG051 2012/2013
#ifndef DU_2_CONSTANTS_HPP
#define DU_2_CONSTANTS_HPP

#include "du2tags.hpp"

namespace details
{
    template <typename CharT, typename Tag>
    struct parsing_constants;

    template <>
    struct parsing_constants<char, POSIX_tag>
    {
        static const char* short_prefix()
        {
            return "-";
        }

        static const char* long_prefix()
        {
            return "--";
        }

        static const char* arg_start()
        {
            return "=";
        }

        static const char* separator()
        {
            return ",";
        }
    };

    template <>
    struct parsing_constants<wchar_t, POSIX_tag>
    {
        static const wchar_t* short_prefix()
        {
            return L"-";
        }

        static const wchar_t* long_prefix()
        {
            return L"--";
        }

        static const wchar_t* arg_start()
        {
            return L"=";
        }

        static const wchar_t* separator()
        {
            return L",";
        }
    };

    template <>
    struct parsing_constants<char, DOS_tag>
    {
        static const char* short_prefix()
        {
            return "/";
        }

        static const char* long_prefix()
        {
            return "/";
        }

        static const char* arg_start()
        {
            return ":";
        }

        static const char* separator()
        {
            return ":";
        }
    };

    template <>
    struct parsing_constants<wchar_t, DOS_tag>
    {
        static const wchar_t* short_prefix()
        {
            return L"/";
        }

        static const wchar_t* long_prefix()
        {
            return L"/";
        }

        static const wchar_t* arg_start()
        {
            return L":";
        }

        static const wchar_t* separator()
        {
            return L":";
        }
    };

    template <>
    struct parsing_constants<char, java_tag>
    {
        static const char* short_prefix()
        {
            return "-";
        }

        static const char* long_prefix()
        {
            return "-";
        }

        static const char* arg_start()
        {
            return ":";
        }

        static const char* separator()
        {
            return ":";
        }
    };

    template <>
    struct parsing_constants<wchar_t, java_tag>
    {
        static const wchar_t* short_prefix()
        {
            return L"-";
        }

        static const wchar_t* long_prefix()
        {
            return L"-";
        }

        static const wchar_t* arg_start()
        {
            return L":";
        }

        static const wchar_t* separator()
        {
            return L":";
        }
    };

    template <typename CharT, typename Tag>
    struct string_constants;

    template <typename Tag>
    struct string_constants<char, Tag>
    {
        static const char* help_switch_title()
        {
            return "Command-line options:\n";
        }

        static const char* placeholder_attr()
        {
            return "attribute";
        }

        static char space()
        {
            return ' ';
        }
    };

    template <typename Tag>
    struct string_constants<wchar_t, Tag>
    {
        static const wchar_t* help_switch_title()
        {
            return L"Command-line options:\n";
        }

        static const wchar_t* placeholder_attr()
        {
            return L"attribute";
        }

        static wchar_t space()
        {
            return L' ';
        }
    };

    template <typename CharT, typename Tag>
    struct separator_constants;

    template <>
    struct separator_constants<char, POSIX_tag>
    {
        static const char* name_separator()
        {
            return ", ";
        }
    };

    template <>
    struct separator_constants<wchar_t, POSIX_tag>
    {
        static const wchar_t* name_separator()
        {
            return L", ";
        }
    };

    template <>
    struct separator_constants<char, DOS_tag>
    {
        static const char* name_separator()
        {
            return " ";
        }
    };

    template <>
    struct separator_constants<wchar_t, DOS_tag>
    {
        static const wchar_t* name_separator()
        {
            return L" ";
        }
    };

    template <>
    struct separator_constants<char, java_tag>
    {
        static const char* name_separator()
        {
            return " | ";
        }
    };

    template <>
    struct separator_constants<wchar_t, java_tag>
    {
        static const wchar_t* name_separator()
        {
            return L" | ";
        }
    };

    template <typename Tag>
    struct numeric_constants;

    template <>
    struct numeric_constants<POSIX_tag>
    {
        static int indent_arg()
        {
            return 2;
        }

        static int indent_help()
        {
            return 30;
        }
    };

    template <>
    struct numeric_constants<DOS_tag>
    {
        static int indent_arg()
        {
            return 2;
        }

        static int indent_help()
        {
            return 12;
        }
    };

    template <>
    struct numeric_constants<java_tag>
    {
        static int indent_arg()
        {
            return 4;
        }

        static int indent_help()
        {
            return 18;
        }
    };
}

#endif // DU_2_CONSTANTS_HPP
