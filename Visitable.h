#pragma once

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

