// Author: Vít Šefl
// Advanced C++, 2nd assignment
// NPRG051 2012/2013
#ifndef DU_2_EXCEPTION_HPP
#define DU_2_EXCEPTION_HPP

#include <exception>

class du2_error : public virtual std::exception
{ };

class argument_error : public virtual du2_error
{ };

class unknown_switch_error : public virtual du2_error
{ };

class unexpected_positional_error : public virtual du2_error
{ };

#endif // DU_2_EXCEPTION_HPP
