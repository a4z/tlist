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
  using contains = std::integral_constant<bool, includes(T{})>;

  static constexpr bool unique()
  {
    auto l = [](auto head, auto... tail)
    {
      if constexpr (sizeof... (tail) == 0)
        return true ;
      else
        return not typelist<decltype(tail)...>::includes (head) &&
               typelist<decltype(tail)...>::unique () ;
    };
    return l(Types{}...) ;
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
  static_assert(t3::contains<t2>::value);
  static_assert(not t3::contains<float>::value);
  static_assert(not t2::contains<t3>::value);

  static_assert(typelist<int, double>::unique ());
  static_assert(not typelist<int, double, int>::unique ());
  static_assert(typelist<char, double, int>::unique ());
  static_assert(not typelist<char, char, double, int>::unique ());
  static_assert(typelist<char, float, double, int>::unique ());
  static_assert(not typelist<char, float, char, int>::unique ());
}
