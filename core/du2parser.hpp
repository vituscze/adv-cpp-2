// Author: Vít Šefl
// Advanced C++, 2nd assignment
// NPRG051 2012/2013
#ifndef DU_2_PARSER_HPP
#define DU_2_PARSER_HPP

#include <algorithm>
#include <exception>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "du2action.hpp"
#include "du2exception.hpp"
#include "du2resulttype.hpp"

// The parser class.
//
// CharT
//   Specifies the underlying character type.
//
// Convention
//   Specifies the convention to be used.
//   Must contain:
//
//     result<CharT> parse_argument(const string_type&)
//       Specifies how to parse the argument according to the convention.
//
//     bool accept_from_args()
//       Specifies, whether the parser should look for more arguments in
//       the argument vector when there is not enough arguments inside
//       result<CharT>.rest.
//
//       For example, under the POSIX convention, "--file" is parsed as
//
//         {type:LONG_NAME, name:"file", rest:{}}
//
//       If the action correponding to "file" switch requires another parameter
//       and accept_from_args() is true, the parser uses next argument in the
//       argument vector, otherwise it fails.
//
//     bool is_length_sensitive()
//       Should the parser distinguish between SHORT_NAME and LONG_NAME.
//
//     typedef ... printer
//       Inner type that specifies how to format the help message.
//       Must contain:
//
//         void help_switch_title(std::basic_ostringstream<CharT>&)
//           (Optional) title of the help message.
//
//         void help_switch(std::basic_ostringstream<CharT>&, const details::action_context<CharT>&)
//           Formats the help message.
//
// UnknownSwitch
//   How to handle unknown switches.
//   Must contain:
//
//     bool handle_error()
//       How to handle unknown switch. When the argument should be tried again
//       as a positional argument, this function returns true.
//
// UpdateArgument
//   What to do with recognized arguments.
//   Must contain:
//
//     iterator update(std::vector<std::basic_string<CharT> >&, iterator, iterator)
//       The pair of iterators specifies the range of recognized arguments.
//       Return iterator specifies where to continue iterating through argument
//       vector.
//
// All following policies must contain:
//   void handle_error()
//
// BadFormat
//   How to handle parsing errors.
//
// UnexpectedPositional
//   How to handle unexpected positional arguments.
//
// Duplicate
//   How to handle duplicate switches.
//
// Missing
//   How to handle missing required arguments or switches.
//
template
    < typename CharT
    , template <typename> class Convention
    , typename UnknownSwitch
    , template <typename> class UpdateArgument
    , typename BadFormat
    , typename UnexpectedPositional
    , typename Duplicate
    , typename Missing
    >
class parser
{
public:
    typedef std::basic_string<CharT> string_type;
    typedef action<CharT>            action_type;

private:
    typedef details::action_context<CharT> action_context;
    typedef std::map<string_type, int>     map_type;
    typedef std::vector<action_context>    action_vector_type;
    typedef std::vector<string_type>       args_vector_type;

public:
    parser()
        : args()
        , switch_actions()
        , positional_actions_req()
        , positional_actions_opt()
        , short_names()
        , long_names()
        , positional_req(0)
        , positional_opt(0)
    { }

    // Collects and formats the help messages from registered switches.
    string_type get_help_message() const
    {
        std::basic_ostringstream<CharT> oss;

        if (switch_actions.size())
        {
            Convention<CharT>::printer::help_switch_title(oss);
            for (const auto& it : switch_actions)
            {
                Convention<CharT>::printer::help_switch(oss, it);
            }
        }

        return oss.str();
    }

    // Adds a required switch.
    void add_req
        ( const string_type& short_name
        , const string_type& long_name
        , const action_type& action
        , const string_type& help_message = string_type()
        )
    {
        add_impl(short_name, long_name, action, help_message, false);
    }

    // Adds an optional switch.
    void add_opt
        ( const string_type& short_name
        , const string_type& long_name
        , const action_type& action
        , const string_type& help_message = string_type()
        )
    {
        add_impl(short_name, long_name, action, help_message, true);
    }

    // Adds a required positional argument.
    void add_pos_req
        ( const action_type& action
        )
    {
        add_pos_impl(action, false);
    }

    // Adds an optional positional argument.
    void add_pos_opt
        ( const action_type& action
        )
    {
        add_pos_impl(action, true);
    }

    // Clears all 'used' flags which track duplicates.
    void reset()
    {
        for (auto& it : switch_actions)
        {
            it.used = false;
        }

        for (auto& it : positional_actions_req)
        {
            it.used = false;
        }

        for (auto& it : positional_actions_opt)
        {
            it.used = false;
        }
    }

    // Clears all data, returning the parser into its default state.
    void clear()
    {
        switch_actions.clear();
        positional_actions_req.clear();
        positional_actions_opt.clear();

        short_names.clear();
        long_names.clear();

        positional_req = 0;
        positional_opt = 0;
    }

    // Sets the underlying argument vector.
    template <typename It>
    void set_args(It start, It end)
    {
        args.clear();
        std::copy(start, end, std::back_inserter(args));
    }

    // Copies the underlying argument vector.
    template <typename It>
    It get_args(It it) const
    {
        return std::copy(args.begin(), args.end(), it);
    }

    // Parses the argument vector according to the rules specified via
    // add_req, add_opt, add_req_pos and add_opt_pos.
    void parse()
    {
        int current_position = 0;
        // Uh oh, declaring these as 'auto' segfaults my gcc.
        typename args_vector_type::iterator it = args.begin();
        typename args_vector_type::iterator it2;

        while (it != args.end())
        {
            // Use the convention to parse current argument.
            auto r = Convention<CharT>::parse_argument(*it);

            bool try_again;

            do
            {
                try
                {
                    try_again = false;

                    switch (r.type)
                    {
                        // How to handle positional arguments.
                        case POSITIONAL:
                        {
                            if (current_position < positional_req + positional_opt)
                            {
                                action_context& ctx =
                                    current_position < positional_req
                                        ? positional_actions_req[current_position]
                                        : positional_actions_opt[current_position - positional_req];

                                ctx.value.accept(*it);
                                ctx.value.execute();
                                ctx.used = true;

                                ++current_position;
                                it2 = it + 1;
                                it = UpdateArgument<CharT>::update(args, it, it2);
                            }
                            else
                            {
                                throw unexpected_positional_error();
                            }

                            break;
                        }
                        // How to handle switches.
                        case SHORT_NAME:
                        case LONG_NAME:
                        {
                            bool length_sensitive = Convention<CharT>::is_length_sensitive();

                            auto end_short = short_names.end();
                            auto end_long = long_names.end();

                            auto found_short = short_names.find(r.name);
                            auto found_long = long_names.find(r.name);

                            action_context* ctx = nullptr;

                            // If the convention is not lenght sensitive, exactly
                            // one match must be found. However, it doesn't matter
                            // whether it comes from the short_names or the
                            // long_names map.
                            if (found_short != end_short
                            && (   (length_sensitive && r.type == SHORT_NAME)
                                || (!length_sensitive && found_long == end_long)))
                            {
                                ctx = &switch_actions[found_short->second];
                            }
                            if (found_long != end_long
                            && (   (length_sensitive && r.type == LONG_NAME)
                                || (!length_sensitive && found_short == end_short)))
                            {
                                ctx = &switch_actions[found_long->second];
                            }

                            if (!ctx)
                            {
                                throw unknown_switch_error();
                            }

                            // This argument was already recognized, handle
                            // duplicate.
                            if (ctx->used)
                            {
                                Duplicate::handle_error();
                            }

                            int req = ctx->value.arg_count();
                            int found_args = static_cast<int>(r.rest.size());
                            int rem = std::distance(it, args.end()) - 1;
                            bool accept_from_args = Convention<CharT>::accept_from_args();

                            // If the argument contained parameters and the
                            // size doesn't match the required number of parameters
                            // or there is not enough arguments in the argument
                            // vector, throw an exception.
                            //
                            // Note, this is to prevent mixing parameters to
                            // switches such as
                            //
                            //   --two_files=file1.txt file2.txt
                            //
                            // Which is unwanted and inconsistent behaviour.
                            // Parser only regonizes these two variants:
                            //
                            //   --two_files=file1.txt,file2.txt
                            //
                            //   --two_files file1.txt file2.txt
                            //     (only when convention allows parameters
                            //     to be extracted from argument vector)
                            //
                            // Separators are convention specific, of course.
                            if  ((found_args >  0 && req != found_args)
                              || (found_args == 0 && req >  rem))
                            {
                                throw unknown_switch_error();
                            }

                            if (found_args > 0)
                            {
                                for (const auto& it3 : r.rest)
                                {
                                    ctx->value.accept(it3);
                                }

                                ctx->value.execute();
                                ctx->used = true;

                                it2 = it + 1;
                                it = UpdateArgument<CharT>::update(args, it, it2);
                            }
                            else if (accept_from_args)
                            {
                                for (auto it3 = it + 1, end = it + 1 + req
                                    ; it3 != end; ++it3)
                                {
                                    ctx->value.accept(*it3);
                                }

                                ctx->value.execute();
                                ctx->used = true;

                                it2 = it + 1 + req;
                                it = UpdateArgument<CharT>::update(args, it, it2);
                            }
                            else
                            {
                                throw unknown_switch_error();
                            }

                            break;
                        }
                        default:
                        {
                            throw std::exception();
                        }
                    }
                }
                catch (unknown_switch_error&)
                {
                    // UnknownSwitch::handle_error specifies whether the
                    // unknown switch should be treated as a positional
                    // argument.
                    bool treat_as_pos = UnknownSwitch::handle_error();
                    if (treat_as_pos)
                    {
                        try_again = true;
                        r.type = POSITIONAL;
                    }
                    else
                    {
                        ++it;
                    }
                }
                catch (unexpected_positional_error&)
                {
                    UnexpectedPositional::handle_error();
                    ++it;
                }
                catch (argument_error&)
                {
                    BadFormat::handle_error();
                    ++it;
                }
            } while (try_again);
        }

        // Traverse all required positional arguments and switches, if any
        // of them was left unused, use Missing::handle_error()
        bool missing = current_position < positional_req;
        for (const auto& it3 : switch_actions)
        {
            if (!it3.used && !it3.optional)
            {
                missing = true;
                break;
            }
        }

        if (missing)
        {
            Missing::handle_error();
        }
    }

private:
    // A helper function which registers a switch.
    void add_impl
        ( const string_type& short_name
        , const string_type& long_name
        , const action_type& action
        , const string_type& help_message
        , bool               optional
        )
    {
        string_type empty;

        action_context c;

        c.value = action;
        c.used = false;
        c.optional = optional;
        c.short_name = short_name;
        c.long_name = long_name;
        c.help_message = help_message;

        int current_pos = static_cast<int>(switch_actions.size());
        switch_actions.push_back(c);

        if (short_name != empty)
        {
            short_names.insert(std::make_pair(short_name, current_pos));
        }

        if (long_name != empty)
        {
            long_names.insert(std::make_pair(long_name, current_pos));
        }
    }

    // A helper function which registers a positional argument.
    void add_pos_impl
        ( const action_type& action
        , bool               optional
        )
    {
        action_context c;

        c.value = action;
        c.used = false;
        c.optional = optional;

        optional
            ? ++positional_opt
            : ++positional_req;

        optional
            ? positional_actions_opt.push_back(c)
            : positional_actions_req.push_back(c);
    }

    args_vector_type args; // Argument vector.

    action_vector_type switch_actions;         // Actions registered to a switch.
    action_vector_type positional_actions_req; // Actions registered to a required argument.
    action_vector_type positional_actions_opt; // Actions registered to an optional argument.

    map_type short_names; // Mapping from short names to switch actions.
    map_type long_names;  // Mapping from long names to switch actions.

    // Note that these two maps map from std::basic_string<CharT> to int,
    // which is a position of the registered action in switch_actions.

    int positional_req; // Number of required positional arguments.
    int positional_opt; // Number of optional positional arguments.
};

#endif // DU_2_PARSER_HPP
