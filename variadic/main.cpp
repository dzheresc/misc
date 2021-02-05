#include <iostream>
#include <vector>
#include <type_traits>

using namespace std;

void print(){}

template <typename First, typename ... Tail>
void print(const First& t, Tail ... p){
    cout << t << ' ';
    print(p...);
}

template <typename T>
void print_impl(const T& t){
    cout << '#' << t << ' ';
}

template <typename ... Params>
void print2(const Params& ... p){
    auto tmp = {(print_impl(p),0)...};
}


/**
 * https://stackoverflow.com/questions/63109411/meta-program-to-remove-adjacent-duplicates-from-compile-time-vector
 *
 */
template<int...>
struct Vector;

template<int Elem, typename Vector>
struct Append;

template<int Elem, int... VecArgs>
struct Append<Elem, Vector<VecArgs...>> {
    using type = Vector<Elem, VecArgs...>;
};

template<typename Vector>
struct uniq;

template<int First, int... Last>
struct uniq<Vector<First, First, Last...>> {
    using type = typename uniq<Vector<First, Last...>>::type;
};

template<int First, int... Last>
struct uniq<Vector<First, Last...>> {
    using type = typename Append<First,
            typename uniq<Vector<Last...>>::type>::type;
};

template<int... I>
struct uniq<Vector<I...>> {
    using type = Vector<I...>;
};

namespace  {
    template<class ... R> struct last_element;

    template<class T> struct last_element<T>{
        using type = T;
    };

    template<class T,class ... R> struct last_element<T,R...>{
        using type = typename last_element<R...>::type;
    };

}
int main() {
//    print("hello", 1, 2.3);
//    print2("hello2", 4, 5.3);
//    cout << is_same<Vector<1,2,3>,uniq<Vector<1,2,2,3>>>::value << '\n';
    cout << typeid(last_element<int,double>::type).name() << '\n';
    return 0;
}
