#pragma once

#include "Visitable.h"

template <typename... Ts> struct Builder {
  template <typename U, typename V> Builder<Ts..., U> field(U(V::*)) {
    return {};
  }

  template <Visitable U, typename V> auto field(U(V::*)) {
    return U::visit(*this);
  }

  using Type = List<Ts...>;
};

template <typename T>
using PackedType = decltype(T::visit(std::declval<Builder<> &>()))::Type;
