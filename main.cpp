#include <type_traits>

template<typename... Types>
struct typelist
{

  template <typename T>
  static constexpr bool includes(T)
  {
    return std::disjunction<std::is_same<T, Types>...>::value ;
  }

  template <typename... T>
  static constexpr bool includes(typelist<T...>)
  {
    return  (includes (T{}) && ...) ;
  }

  template <typename T>
  using contains = std::disjunction<std::is_same<T, Types>...>;

  template <typename... T>
  using contains_list = std::conjunction<contains<T>...>;

  template <typename... T>
  static constexpr bool test(typelist<T...>)
  {
    return std::conjunction<contains <T>...>::value ;
  }

};

int main()
{
  using t1 = typelist<int, double>;
  using t2 = typelist<double, int>;
  using t3 = typelist<int, double, char>;

  static_assert(t1::includes (int{}));
  static_assert(not t1::includes (float{}));
  static_assert(t1::includes (t2{}));
  static_assert(t2::includes (t1{}));
  static_assert(t3::includes (t2{}));
  static_assert(not t2::includes (t3{}));


  static_assert(t1::contains<int>::value);
  //static_assert(t3::contains_list<t2>::value);
  static_assert(t3::test(t2{})); // why this, but not the line above?

}
