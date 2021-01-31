#include <iostream>

double mult(double x, long n){
    if (n == 0)
        return 1;

    auto y = mult(x*x,n/2);
    if (n & 1)
        return x*y;
    else
        return y;
}

double frac(double x, long n){
    return 1/mult(x,-labs(n));
}
int main() {
    for(int i=-5; i <= 5;i++)
        std::cout << (i < 0 ? frac(2,i):mult(2,i)) << std::endl;
//    std::cout << mult(2,0) << std::endl;
//    std::cout << mult(2,1) << std::endl;
//    std::cout << mult(2,2) << std::endl;
//    std::cout << mult(2,3) << std::endl;
//    std::cout << mult(2,4) << std::endl;
//    std::cout << mult(2,5) << std::endl;
    return 0;
}
