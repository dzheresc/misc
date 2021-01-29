#include <iostream>
#include <vector>
#include <array>

using namespace std;

/*
 * abcd*11
 *
 * 43210
 *  abcd
 * abcd
 * -----
 *
 * v0 = v0 // always 1
 *
 * prev = 0
 * for i=1;i < n;i++
 *  prev
 *  tmp = v[i-1];
 *  x = v[i] + tmp + c
 *  prev = v[i]
 *  v[i] = x / 10
 *  c = x % 10
 *

 */
void mul11(vector<uint8_t>& v){
    int carry = 0;

    int sz = v.size();
    for(int i=1; i< sz;i++){

        //v[i] =
    }
}

vector<uint8_t> pow11(int n){
    if (n == 0) return {1};
    vector<uint8_t> v{1,1};
    if (n == 1) return v;

    for(int i=1;i < n;i++) {
        int sz = v.size();
        v.push_back(0);

        int prev = v[0];
        int carry = 0;
        for (int i = 1; i <= sz; i++) {
            int val = v[i] + prev + carry;
            prev = v[i];
            v[i] = val % 10;
            carry = val / 10;
        }
    }
    return v;
}

void print(vector<uint8_t> v){
    for(auto it = v.rbegin(); it != v.rend(); it++)
        cout << (int)*it;

    cout << '\n';
}
int main() {
    print(pow11(0));
    print(pow11(1));
    print(pow11(2));
    print(pow11(3));
    print(pow11(4));
    print(pow11(5));
    print(pow11(6));
    print(pow11(7));
    print(pow11(8));
    print(pow11(9));
    print(pow11(10));
    return 0;
}
