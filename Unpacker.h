#pragma once

#include "Visitable.h"
#include "List.h"
#include "PackedType.h"

template<typename T, typename L> struct Unpacker { };

template<typename T, typename... Us>
struct Unpacker<T, List<Us...>>
{
    T& a;
    const List<Us...>& total;

    Unpacker(T& a_in, const List<Us...>& total_in) : a(a_in), total(total_in) { }

    template<typename X>
    auto expand(X& x)
    {
      return Unpacker<X, List<Us...>>(x, total);
    }

    template<typename U, typename V>
    auto field(U (V::*e))
    {
        if constexpr (Visitable<U>)
        {
          Unpacker<U, List<Us...>> visitor(a.*e, total);
          auto visitor_new = (a.*e).visit(visitor);
          return visitor_new.expand(a);
        }
        else
        {
          static_assert(std::same_as<typename List<Us...>::Head, U>);

          // std::cout << "unpacking " << total.head << std::endl;
          a.*e = total.head;
          return Unpacker<T, typename List<Us...>::Tail>(a, static_cast<const typename List<Us...>::Tail&>(total));
        }
    }

    // using Type = List<Ts...>;
};

template<typename T> struct Unpacker<T, List<>>
{
    const List<>& total;

    Unpacker(T& a_in, const List<>& total_in) : total(total_in) { }

    template<typename X>
    auto expand(X& x)
    {
      return Unpacker<X, List<>>(x, total);
    }
};

template <typename T> T Unpack(const PackedType<T> &list) {
  T x;
  Unpacker<T, PackedType<T>> unpacker(x, list);
  T::visit(unpacker);
  return x;
}
