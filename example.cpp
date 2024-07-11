#include <cassert>
#include <utility>

// The "library" part of the code

struct MockVisitor
{
    MockVisitor field(auto)
    {
      return std::declval<MockVisitor>();
    } 
};

template<typename T>
concept Visitable = requires(T t)
{
  t.visit(std::declval<MockVisitor>());
};

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

// The "user" part.

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
int main() {
  test1();
  test2();
}
