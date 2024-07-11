#include <cassert>
#include <string>

#include "List.h"
#include "PackedType.h"
#include "Packer.h"

struct A
{
    int x;

    static auto visit(auto visitor)
    {
        return visitor.field(&A::x);
    }
};

#if 1
// Test a single field.
void test1()
{
    A a{17};
    List<int> list;
    Packer<A, List<int>> packer(a, list);
    a.visit(packer);

    assert(list.head == 17);
}
#endif

struct B
{
    std::string x;
    double y;

    static auto visit(auto visitor)
    {
        return visitor.field(&B::x)
                      .field(&B::y);
    }
};

#if 1
// Test two fields.
void test2()
{
    B b{"hello", 3.14};
    List<std::string, double> list;
    Packer<B, List<std::string, double>> packer(b, list);
    b.visit(packer);

    assert(list.head == "hello");
    assert(list.GetTail().head == 3.14);
}
#endif

struct C
{
    A x;
    B y;

    static auto visit(auto visitor)
    {
        return visitor.field(&C::x)
                      .field(&C::y);
    }
};

#if 1
// Test nested types.
void test3()
{
    C c{{44}, {"hi", 2.72}};
    List<int, std::string, double> list;
    Packer<C, List<int, std::string, double>> packer(c, list);
    c.visit(packer);

    assert(list.head == 44);
    assert(list.GetTail().head == "hi");
    assert(list.GetTail().GetTail().head == 2.72);
}
#endif

struct D : public B
{
  int z;

  static auto visit(auto visitor)
  {
    return B::visit(visitor)
             .field(&D::z);
  }
};

#if 1
// Test inheritance
void test4()
{
    D d{{"yo!", 2.0}, 11};
    List<std::string, double, int> list;
    Packer<D, List<std::string, double, int>> packer(d, list);
    d.visit(packer);

    assert(list.head == "yo!");
    assert(list.GetTail().head == 2.0);
    assert(list.GetTail().GetTail().head == 11);
}
#endif

#if 1
void test5()
{
    C c{{42}, {"ho", 3.72}};
    PackedType<C> list;
    Packer<C, PackedType<C>> packer(c, list);
    c.visit(packer);

    assert(list.head == 42);
    assert(list.GetTail().head == "ho");
    assert(list.GetTail().GetTail().head == 3.72);
}
#endif

#if 1
void test6()
{
    D d{{"yow", 2.1}, -12};
    PackedType<D> list;
    Packer<D, PackedType<D>> packer(d, list);
    d.visit(packer);

    assert(list.head == "yow");
    assert(list.GetTail().head == 2.1);
    assert(list.GetTail().GetTail().head == -12);
}
#endif

void test7()
{
    D d{{"yow!", 2.3}, -13};
    auto list = Pack(d);

    assert(list.head == "yow!");
    assert(list.GetTail().head == 2.3);
    assert(list.GetTail().GetTail().head == -13);
}

int main()
{
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();
}
