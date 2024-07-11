#pragma once

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
