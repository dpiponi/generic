#include <cassert>
#include <string>

#include "List.h"
#include "PackedType.h"
#include "Packer.h"
#include "Unpacker.h"

struct A {
  int x;

  static auto visit(auto visitor) { return visitor.field(&A::x); }
};

// Test a single field.
void test1() {
  A a{17};
  List<int> list;
  Packer<A, List<int>> packer(a, list);
  a.visit(packer);

  assert(list.head == 17);
}

struct B {
  std::string x;
  double y;

  bool operator==(const B &) const = default;

  static auto visit(auto visitor) { return visitor.field(&B::x).field(&B::y); }
};

#if 1
// Test two fields.
void test2() {
  B b{"hello", 3.14};
  List<std::string, double> list;
  Packer<B, List<std::string, double>> packer(b, list);
  b.visit(packer);

  assert(list.head == "hello");
  assert(list.GetTail().head == 3.14);
}
#endif

struct C {
  A x;
  B y;

  static auto visit(auto visitor) { return visitor.field(&C::x).field(&C::y); }
};

#if 1
// Test nested types.
void test3() {
  C c{{44}, {"hi", 2.72}};
  List<int, std::string, double> list;
  Packer<C, List<int, std::string, double>> packer(c, list);
  c.visit(packer);

  assert(list.head == 44);
  assert(list.GetTail().head == "hi");
  assert(list.GetTail().GetTail().head == 2.72);
}
#endif

struct D : public B {
  int z;

  bool operator==(const D &) const = default;

  static auto visit(auto visitor) { return B::visit(visitor).field(&D::z); }
};

#if 1
// Test inheritance
void test4() {
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
void test5() {
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
void test6() {
  D d{{"yow", 2.1}, -12};
  PackedType<D> list;
  Packer<D, PackedType<D>> packer(d, list);
  d.visit(packer);

  assert(list.head == "yow");
  assert(list.GetTail().head == 2.1);
  assert(list.GetTail().GetTail().head == -12);
}
#endif

void test7() {
  D d{{"yow!", 2.3}, -13};
  auto list = Pack(d);

  assert(list.head == "yow!");
  assert(list.GetTail().head == 2.3);
  assert(list.GetTail().GetTail().head == -13);
}

void test8() {
  A a;
  List<int> list{{}, 170};
  Unpacker<A, List<int>> unpacker(a, list);
  a.visit(unpacker);

  assert(a.x == 170);
}

void test9() {
  B b;
  List<std::string, double> list{{{}, 2.14}, std::string("bye")};
  Unpacker<B, List<std::string, double>> unpacker(b, list);
  b.visit(unpacker);

  assert(b.x == "bye");
  assert(b.y == 2.14);
}

void test10() {
  D d{{"hey, there!", 1.0}, 111};
  auto list = Pack(d);
  D e = Unpack<D>(list);

  assert(d == e);
}

struct E
{
  float x;
  float y;
  float z;

  bool operator==(const E&) const = default;

  static auto visit(auto visitor)
  {
    return visitor.field(&E::x)
                  .field(&E::y)
                  .field(&E::z);
  }
};

struct F
{
  float r;
  float g;
  float b;

  bool operator==(const F&) const = default;

  static auto visit(auto visitor)
  {
    return visitor.field(&F::r)
                  .field(&F::g)
                  .field(&F::b);
  }
};

void test11()
{
  E v{1., 2., 3.};
  F c = Unpack<F>(Pack(v));

  assert(c.r == v.x);
  assert(c.g == v.y);
  assert(c.b == v.z);
}

int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();
  test8();
  test9();
  test10();
  test11();
}
