#include <iostream>
#include <utility>

#include "List.h"
#include "Visitable.h"
#include "Adder.h"
#include "Accumulator.h"

template<typename T>
struct Add
{
  T& a;
  T& b;
  T& result;

  template<typename U>
  Add<T> field(U (T::*e))
  {
    result.*e = a.*e + b.*e;
    return *this;
  }
};

struct A
{
  float x;
  float y;
  float z;

  static auto visit(auto visitor)
  {
    return visitor.field(&A::x)
                  .field(&A::y)
                  .field(&A::z);
  }
};

struct B
{
  uint8_t r;
  uint8_t g;
  uint8_t b;

  static auto visit(auto visitor)
  {
    return visitor.field(&B::r)
                  .field(&B::g)
                  .field(&B::b);
  }
};

template<Visitable T>
T operator+(T& a, T& b)
{
    T c;

    Add<T> add{a, b, c};
    a.visit(add);

    return c;
}

void test3()
{
    A a{1., 2., 3.};
    A b{10., 20., 30.};
    A result;

    Add<A> add{a, b, result};
    a.visit(add);
    std::cout << add.result.x << ' ' << add.result.y << ' ' << add.result.z << std::endl;

    B p{1, 2, 3};
    B q{100, 101, 102};
    B r = p + q;
    std::cout << r.r << ' ' << r.g << ' ' << r.b << std::endl;
}

int main()
{
  test3();
}
