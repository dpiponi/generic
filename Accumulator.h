#pragma once

// ADDER

template<typename T, typename... Us> struct Adder2 { };

template<typename T, typename U, typename... Us>
struct Adder2<T, U, Us...>
{
    T& a;
    T& total;

    Adder2(T& a_in, T& total_in) : a(a_in), total(total_in) { }

    Adder2 field(U (T::*e))
    {
        total.*e += a.*e;
        return *this;
    }

    // using Type = List<Ts...>;
};

template<typename T> struct Adder2<T>
{
    Adder2(T& a_in, List<>& total_in) { }
};
