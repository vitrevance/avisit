#include <gtest/gtest.h>

#include <avisit.hpp>
#include <memory>
#include <vector>

struct Circle : avisit::VisitableOf<Circle> {
  std::string name = "Circle";
};
struct Square : avisit::VisitableOf<Square> {
  std::string name = "Square";
};

TEST(simple, Simple) {
  std::vector<std::unique_ptr<avisit::Visitable>> shapes;
  shapes.push_back(std::make_unique<Circle>());
  shapes.push_back(std::make_unique<Square>());

  for (auto& shape : shapes) {
    shape->Visit<Circle, Square>([](auto ptr) {
      if constexpr (std::is_same_v<decltype(ptr), Circle*>) {
        ASSERT_EQ(ptr->name, "Circle");
      } else {
        ASSERT_EQ(ptr->name, "Square");
      }
    });
  }
}