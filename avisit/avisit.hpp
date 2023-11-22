#ifndef AVISIT_HPP
#define AVISIT_HPP

#include <concepts>
#include <initializer_list>

namespace avisit {

template <typename T>
concept PureType = std::same_as<T, std::remove_cvref_t<T>>;

template <PureType T>
struct VisitableOf;

template <typename T>
class TypeKey {
  consteval TypeKey() = default;
  friend class VisitableOf<T>;
  friend struct TypeTag;
  static inline std::byte placeholder{};
};

struct Visitable;

struct TypeTag {
  template <typename T>
  consteval TypeTag(TypeKey<T>) {  // NOLINT
    static_assert(sizeof(id) == sizeof(&TypeKey<T>::placeholder));
    static_assert(alignof(decltype(id))
                  == alignof(decltype(&TypeKey<T>::placeholder)));
    id = &TypeKey<T>::placeholder;
  }
  template <typename T>
  consteval static void* GetId() {
    return TypeTag{TypeKey<T>{}}.id;
  }
  void* id;
};

struct Visitable {
  template <typename... Expected>
  void Visit(auto&& lambda)
    requires(... and std::invocable<decltype(lambda), Expected*>)
            && (... and std::is_base_of_v<Visitable, Expected>)
            && (... and std::same_as<Expected, std::remove_cvref_t<Expected>>)
            && (sizeof...(Expected) > 0)
  {
    std::initializer_list<int>({(type_.id == TypeTag::GetId<Expected>()
                                 ? (lambda(static_cast<Expected*>(this))),
                                 0 : 0)...});
  }

  template <PureType T>
  friend class VisitableOf;

 private:
  explicit consteval Visitable(TypeTag type)
      : type_(type) {
  }
  TypeTag type_;
};

template <PureType T>
struct VisitableOf : Visitable {
  friend T;

 private:
  consteval VisitableOf()
      : Visitable(TypeKey<T>{}) {
  }
};

}  // namespace avisit

#endif