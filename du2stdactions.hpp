// Author: Vít Šefl
// Advanced C++, 2nd assignment
// NPRG051 2012/2013
#ifndef DU_2_STD_ACTIONS_HPP
#define DU_2_STD_ACTIONS_HPP

#include <sstream>
#include <string>

#include "core/du2action.hpp"
#include "core/du2exception.hpp"

// Standard set action.
// Upon executing this action, ref is set to value.
template <typename CharT, typename T>
class set_action : public inner_action<CharT>
{
public:
    virtual int arg_count() const
    { return 0; }

    virtual void accept(const std::basic_string<CharT>&)
    { }

    virtual void execute()
    { ref = value; }

    set_action(T& ref, const T& value)
        : ref(ref)
        , value(value)
    { }

private:
    T& ref;
    const T& value;
};

// Convenience function for constructring set actions.
template <typename CharT, typename T>
action<CharT> make_set_action(T& ref, const T& value)
{
    return action<CharT>(std::make_shared<set_action<CharT, T> >(ref, value));
}

// Standard get action.
// Expects a single string argument and converts it to desired target type
// using stream extraction operator.
template <typename CharT, typename T>
class get_action : public inner_action<CharT>
{
public:
    virtual int arg_count() const
    { return 1; }

    virtual void accept(const std::basic_string<CharT>& s)
    {
        std::basic_istringstream<CharT> iss(s);
        iss >> ref;

        if (iss.fail())
        {
            throw argument_error();
        }
    }

    virtual void execute()
    { }

    get_action(T& ref)
        : ref(ref)
    { }

private:
    T& ref;
};

// Convenience function for constructring get actions.
template <typename CharT, typename T>
action<CharT> make_get_action(T& ref)
{
    return action<CharT>(std::make_shared<get_action<CharT, T> >(ref));
}

#endif // DU_2_STD_ACTIONS_HPP
