#include <iostream>
#include <vector>

using namespace std;

void print(){}

template <typename T, typename ... Params>
void print(const T& t, Params ... p){
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

int main() {
    print("hello", 1, 2.3);
    print2("hello2", 4, 5.3);
    return 0;
}
