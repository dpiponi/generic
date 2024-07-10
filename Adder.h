#pragma once

template<typename T>
struct Adder
{
    T& a;
    int total;

    Adder(T& a_in) : a(a_in), total(0) { }

    template<typename U>
    Adder<T> field(U (T::*e))
    {
        if constexpr (std::is_same<U, int>::value)
        {
            total += a.*e;
        }
        return *this;
    }

    void run()
    {
        a.visit(*this);
    }
};
