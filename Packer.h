#pragma once

#include "List.h"
#include "Visitable.h"
// #include <iostream>

template <typename T, typename L> struct Packer {};

template <typename T, typename... Us> struct Packer<T, List<Us...>> {
  const T &a;
  List<Us...> &total;

  Packer(const T &a_in, List<Us...> &total_in) : a(a_in), total(total_in) {}

  template <typename X> auto expand(X &x) {
    return Packer<X, List<Us...>>(x, total);
  }

  template <typename U, typename V> auto field(U(V::*e)) {
    if constexpr (Visitable<U>) {
      Packer<U, List<Us...>> visitor(a.*e, total);
      auto visitor_new = (a.*e).visit(visitor);
      return visitor_new.expand(a);
    } else {
      static_assert(std::same_as<typename List<Us...>::Head, U>);

      total.head = a.*e;
      // std::cout << "packed " << total.head << std::endl;
      return Packer<T, typename List<Us...>::Tail>(
          a, static_cast<typename List<Us...>::Tail &>(total));
    }
  }
};

template <typename T> struct Packer<T, List<>> {
  List<> &total;

  Packer(const T &a_in, List<> &total_in) : total(total_in) {}

  template <typename X> auto expand(X &x) {
    return Packer<X, List<>>(x, total);
  }
};

template <typename T> PackedType<T> Pack(const T &x) {
  PackedType<T> list;
  Packer<T, PackedType<T>> packer(x, list);
  T::visit(packer);
  return list;
}
