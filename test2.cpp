#include <cassert>
#include <iostream>
#include <utility>

#include "Packer.h"
#include "Unpacker.h"
#include "Visitable.h"

// Example 1
// Getting mathematical operators on types for "free".
// Here is the way you implement an addition visitor.
// (And of course we could generalise this.)
template <typename T> struct Add {
  const T &a;
  const T &b;
  T &result;

  template <typename U> Add<T> field(U(T::*e)) {
    result.*e = a.*e + b.*e;
    return *this;
  }
};

template <Visitable T> T operator+(const T &a, const T &b) {
  T c;

  Add<T> add{a, b, c};
  a.visit(add);

  return c;
}

// And now we define some types.
template <typename T> struct Vector {
  T x;
  T y;
  T z;

  bool operator==(const Vector &) const = default;

  // This is the extra burden we put on developers.
  // Each type needs a visit method to enumerate the
  // fields. But once this is done a lot comes
  // for free.
  static auto visit(auto visitor) {
    return visitor.field(&Vector::x).field(&Vector::y).field(&Vector::z);
  }
};

template <typename T> struct Color {
  T r;
  T g;
  T b;

  bool operator==(const Color &) const = default;

  static auto visit(auto visitor) {
    return visitor.field(&Color::r).field(&Color::g).field(&Color::b);
  }
};

// Both Vector and Color have the + operator now:
void test1() {
  Vector<double> p{1., 2., 3.};
  Vector<double> q{100., 101., 102.};
  Vector<double> v = p + q;
  assert((v == Vector<double>{101., 103., 105.}));
}

void test2() {
  Color<uint8_t> p{1, 2, 3};
  Color<uint8_t> q{100, 101, 102};
  Color<uint8_t> color = p + q;
  assert((color == Color<uint8_t>{101, 103, 105}));
}

// We can convert between types that have the same underlying types.
// Note this will work even if the types have different layouts
// (ie. when casting a pointer isn't safe).
void test3() {
  Color<float> color{1.f, 2.f, 3.f};
  Vector<float> v = Unpack<Vector<float>>(Pack(color));
  assert((v == Vector<float>{1.f, 2.f, 3.f}));
}

// A more complex example
// Testing inheritance...
struct Convoluted1 : public Color<int> {
  float u, v, w;

  bool operator==(const Convoluted1 &) const = default;

  static auto visit(auto visitor) {
    // To handle derived types we pass the visitor to the
    // parent class's visit() method.
    return Color<int>::visit(visitor)
        .field(&Convoluted1::u)
        .field(&Convoluted1::v)
        .field(&Convoluted1::w);
  }
};

// ... and testing nesting.
struct Convoluted2 {
  Vector<int> a;
  Vector<float> b;

  bool operator==(const Convoluted2 &) const = default;

  static auto visit(auto visitor) {
    return visitor.field(&Convoluted2::a).field(&Convoluted2::b);
  }
};

void test4() {
  Convoluted1 a{{1, 2, 3}, 4., 5., 6.};
  Convoluted2 b = Unpack<Convoluted2>(Pack(a));
  assert((b == Convoluted2{{1, 2, 3}, {4., 5., 6.}}));
}

int main() {
  test1();
  test2();
  test3();
  test4();
}
