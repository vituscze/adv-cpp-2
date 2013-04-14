// Author: Vít Šefl
// Advanced C++, 2nd assignment
// NPRG051 2012/2013
#ifndef DU_2_ACTION_HPP
#define DU_2_ACTION_HPP

#include <memory>
#include <string>

// An inner, polymorphic base for actions.
template <typename CharT = char>
class inner_action
{
public:
    // How many parameters are required.
    virtual int arg_count() const = 0;

    // How to handle each parameter.
    virtual void accept(const std::basic_string<CharT>&) = 0;

    // Called once all parameters have been provided.
    virtual void execute() = 0;
};

// A non-polymorphic wrapper around inner_action. Note that std::shared_ptr
// is used. This allows multiple switches to share the same underlying action.
// This behaviour should not be normally needed, but is provided for convenience
// and flexibility.
template <typename CharT = char>
class action
{
public:
    int arg_count() const
    { return impl->arg_count(); }

    void accept(const std::basic_string<CharT>& s)
    { impl->accept(s); }

    void execute()
    { impl->execute(); }

    action()
        : impl(nullptr)
    { }

    action(inner_action<CharT>* ptr)
        : impl(ptr)
    { }

    action(std::shared_ptr<inner_action<CharT> > ptr)
        : impl(ptr)
    { }

private:
    std::shared_ptr<inner_action<CharT> > impl;
};

namespace details
{
    template <typename CharT>
    struct action_context
    {
        typedef std::basic_string<CharT> string_type;

        action<CharT> value;

        bool used;
        bool optional;

        string_type short_name;
        string_type long_name;

        string_type help_message;
    };
}

#endif // DU_2_ACTION_HPP
