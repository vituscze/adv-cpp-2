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
        < wchar_t
        , POSIX
        , unknown_switch_throw_exception
        , argument_leave
        , bad_format_throw_exception
        , unexpected_arg_throw_exception
        , duplicate_throw_exception
        , missing_throw_exception
        > p;

    std::wstring _args[] =
        {L"--count", L"25", L"-f", L"name.exe", L"--size=5"};
    std::vector<std::wstring> args(_args, _args + lengthof(_args));

    std::wstring file_name;
    std::size_t count = 0;
    std::size_t size = 0;
    bool verbose = false;

    p.add_req(L"f", L"file", make_get_action<wchar_t>(file_name)
        , L"Sets the file name.");
    p.add_req(L"c", L"count", make_get_action<wchar_t>(count)
        , L"Sets the count.\nOnly positive numbers are accepted.");
    p.add_req(L"", L"size", make_get_action<wchar_t>(size)
        , L"Sets the size.\nOnly numbers greater than zero are accepted.");
    p.add_opt(L"v", L"verbose", make_set_action<wchar_t>(verbose, true)
        , L"Turns on the verbose mode.");

    p.set_args(args.begin(), args.end());

    p.parse();

    std::wcout << L"file name: " << file_name << L"\n"
               << L"count: "     << count     << L"\n"
               << L"size: "      << size      << L"\n"
               << L"verbose: "   << std::boolalpha << verbose;

    std::wcout << L"\n\n" << p.get_help_message();
}
