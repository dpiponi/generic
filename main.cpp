#include <iostream>
#include <utility>

#include "List.h"

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

#include "Visitable"
#include "Adder.h"

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

#include "Packer.h"
#include "Unpacker.h"
#include "Accumulator.h"

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
    Packer<A, List<int, int>> x(a, total);
    a.visit(x);
#if 0
    Unpacker<A, int, int> y(b, total);
    b.visit(y);
#endif

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

#if 1
struct C : public A
{
    float w;
    A v;

    auto visit(auto visitor)
    {
        return A::visit(visitor)
                 .field(&C::w)
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
    Unpacker<A, int, int> y(b, total);
    auto y_new = a.visit(y);
    std::cout << b.xx << ' ' << b.yy << std::endl;
#endif

    B c[] = {{1, 2, 3}, {10, 20, 30}, {100, 200, 300}};

    B total = test3(c, 3);

    std::cout << total.xx << ' ' << total.yy << ' ' << total.zz << std::endl;

#if 1
    C cc{{1, 2}, 30.0f, {3, 4}};
    List<int, float, int, int, float, int> packed;
    Packer<C, List<int, float, int, int, float, int>> packer(cc, packed);
    cc.visit(packer);
    std::cout << packed.GetTail().GetTail().GetTail().head << std::endl;
    packed.GetTail().GetTail().head = 99.0f;

    Unpacker<C, List<int, float, int, int, float, int>> unpacker(cc, packed);
    cc.visit(unpacker);
    std::cout << "cc.w = " << cc.w << std::endl;
#endif

#if 0
    C cc{{1, 2}, {3, 4}};
    List<int, int, int, int> unpacked;
    Unpacker<C, List<int, int, int, int>> unpacker(cc, unpacked);
    cc.visit(unpacker);
    std::cout << unpacker.GetTail().GetTail().GetTail().head << std::endl;
#endif

    //std::remove_reference<decltype(a.visit(x))>::type::Type y;
}
