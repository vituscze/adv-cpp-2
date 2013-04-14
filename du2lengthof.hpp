// Author: Vít Šefl
// Advanced C++, 2nd assignment
// NPRG051 2012/2013
#ifndef DU_2_LENGHT_OF_HPP
#define DU_2_LENGHT_OF_HPP

template <typename T, int N>
int lengthof(const T(&)[N])
{
    return N;
}

#endif // DU_2_LENGHT_OF_HPP
