#include <iostream>
#include <utility>

struct A
{
    int xx;
    int yy;

    //template<typename Visitor>
    auto visit(auto visitor)
    {
        return visitor.field(&A::xx)
                      .field(&A::yy);
    }
};

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

template<typename T>
concept Visitable = requires(T t)
{
  t.visit(std::declval<Adder<T>>());
};

#if 1
template<typename... Ts> struct List;

template<typename T, typename... Ts> struct List<T, Ts...> : public List<Ts...>
{
    T head;


    using Head = T;
    using Tail = List<Ts...>;
    
    Tail& GetTail()
    {
        return *static_cast<Tail*>(this);
    }
};

template<> struct List<>
{
};

template<typename T, typename... Ts> struct Builder
{
    template<typename U>
    Builder<T, Ts..., U>& field(U (T::*e)) const
    {
        // std::unreachable();
    }

    using Type = List<Ts...>;

};

template<typename T>
using Generic = std::remove_reference<decltype(
    std::declval<T>().visit(std::declval<Builder<T>&>())
    )>::type::Type;

// PACKER

template<typename T, typename... Us> struct Packer { };

template<typename T, typename U, typename... Us>
struct Packer<T, U, Us...>
{
    T& a;
    List<U, Us...>& total;

    Packer(T& a_in, List<U, Us...>& total_in) : a(a_in), total(total_in) { }

    Packer<T, Us...> field(U (T::*e))
    {
        total.head = a.*e;
        return Packer<T, Us...>(a, static_cast<List<Us...>&>(total));
    }

    // using Type = List<Ts...>;
};

template<typename T> struct Packer<T>
{
    Packer(T& a_in, List<>& total_in) { }
};

#if 0
template<typename U, typename...Us>
Packer<T, U, List<Us...>> make_accumulator(T& a, List<U, Us...>& total)
{
    return Packer<T, U, Us...>(a, total);
}
#endif
#endif


// UNPACKER

template<typename T, typename... Us> struct UnPacker { };

template<typename T, typename U, typename... Us>
struct UnPacker<T, U, Us...>
{
    T& a;
    List<U, Us...>& total;

    UnPacker(T& a_in, List<U, Us...>& total_in) : a(a_in), total(total_in) { }

    UnPacker<T, Us...> field(U (T::*e))
    {
        a.*e = total.head;
        return UnPacker<T, Us...>(a, static_cast<List<Us...>&>(total));
    }

    // using Type = List<Ts...>;
};

template<typename T> struct UnPacker<T>
{
    UnPacker(T& a_in, List<>& total_in) { }
};

#if 0
template<typename U, typename...Us>
UnPacker<T, U, List<Us...>> make_accumulator(T& a, List<U, Us...>& total)
{
    return UnPacker<T, U, Us...>(a, total);
}
#endif

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

int test(A& u)
{
    Adder<A> adder(u);
    adder.run();
    return adder.total;
}

A test2(A& a)
{
    A b;
    List<int, int> total;
    Packer<A, int, int> x(a, total);
    a.visit(x);
    UnPacker<A, int, int> y(b, total);
    b.visit(y);

    return b;
}

struct B : public A
{
    int zz;

    //template<typename Visitor>
    auto visit(auto visitor)
    {
        return A::visit(visitor).field(&B::zz);
    }
};

template<typename T>
T test3(T* as, int n)
{
    T total(as[0]);

    for (int i = 1; i < n; ++i)
    {
        Adder2<T, int, int> x(as[i], total);
        total.visit(x);
    }

    return total;
}

#if 0
struct C
{
    A u;
    A v;

    auto visit(auto visitor)
    {
        return visitor.field(&C::u)
                      .field(&C::v);
    }
};
#endif

int main()
{
    A a{1, 2};

#if 0
    Adder<A> adder(a);
    auto new_adder = a.visit(adder);
    std::cout << new_adder.total << std::endl;
#endif

#if 0
    List<int, int> total;
    Packer<A, int, int> x(a, total);
    auto x_new = a.visit(x);

    std::cout << x.total.head << ' ' << x.total.GetTail().head << std::endl;

    A b;
    UnPacker<A, int, int> y(b, total);
    auto y_new = a.visit(y);
    std::cout << b.xx << ' ' << b.yy << std::endl;
#endif

    B c[] = {{1, 2, 3}, {10, 20, 30}, {100, 200, 300}};

    B total = test3(c, 3);

    std::cout << total.xx << ' ' << total.yy << ' ' << total.zz << std::endl;

#if 0
    C cc{{1, 2}, {3, 4}};
    List<int, int, int, int> unpacked;
    UnPacker<C, int, int, int, int> unpacker(cc, unpacked);
    cc.visit(unpacker);
    std::cout << unpacker.GetTail().GetTail().GetTail().head << std::endl;
#endif

    //std::remove_reference<decltype(a.visit(x))>::type::Type y;
}
