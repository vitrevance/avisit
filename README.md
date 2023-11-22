# avisit - Acyclic Visitor Pattern for Flat Class Hierarchies

avisit is a single header C++ library that provides an implementation of the Acyclic Visitor Pattern for flat class hierarchies. It allows you use templated lambdas for visiting objects with zero virtual calls.

## Usage

To use avisit, you first need to define your visitable classes using the VisitableOf template:
```cpp
struct Circle : avisit::VisitableOf<Circle> {
  std::string name = "Circle";
};
struct Square : avisit::VisitableOf<Square> {
  std::string name = "Square";
};
struct Banana : avisit::VisitableOf<Banana> {
  int weight = 1;
};
```

Then, you can call the Visit method on each object with common base class ```avisit::Visitable```, passing in a lambda that defines the behavior for each visitor:

```cpp
std::vector<std::unique_ptr<avisit::Visitable>> shapes;
shapes.push_back(std::make_unique<Circle>());
shapes.push_back(std::make_unique<Square>());
shapes.push_back(std::make_unique<Banana>());


for (auto& shape : shapes) {
    // Can work with Circles and Squares but not Bananas
  shape->Visit<Circle, Square>([](auto ptr) {
    if constexpr (std::is_same_v<decltype(ptr), Circle*>) {
      ASSERT_EQ(ptr->name, "Circle");
    } else {
      ASSERT_EQ(ptr->name, "Square");
    }
  });
}
```

In this example, we define a lambda that checks the type of the visitable object and asserts that its name member variable is correct.

Template parameters for Visit function are expected types that can be visited by the visitor implementation (the lambda), if actual type is not in the list then lambda is not called and object is effectively ignored.
