#include <iostream>
#include <variant>
#include <vector>
#include <string>

// source: https://www.bfilipek.com/2019/02/2lines3featuresoverload.html

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

using var_t = std::variant<int, long, double, const std::string>;

class Vis {
public:
  void operator()(int& arg) const { std::cout << "int:" << arg << '\n';}
  void operator()(long& arg) const { std::cout << "long:" << arg << '\n';}
  void operator()(double& arg) const { std::cout << "double:" << arg << '\n';}
  void operator()(const std::string& arg) const { std::cout << "string*:" << arg << '\n';}
};


void test_var()
{
  std::vector<var_t> vec {1,2l,3.4,"5"};
  for(auto& v: vec) {
    std::visit(Vis(), v);

    std::visit(overloaded{
      [](int &arg) { std::cout << arg << '\n'; },
      [](long &arg) { std::cout << arg << 'L' << '\n'; },
      [](double &arg) { std::cout << std::fixed << arg << '\n'; },
      [](const std::string &arg) { std::cout << '"' << arg << '"' << '\n'; },
    }, v);
  }
}
