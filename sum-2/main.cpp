#include <iostream>
#include <vector>

using namespace std;

void print(const vector<int>& v){
    for(auto it = v.rbegin(); it != v.rend(); ++it){
        cout << *it << ' ';
    }
    cout << '\n';
}

long long number(const vector<int>& v){
    long long x{0};
    int exp = 1;
    for(auto it = v.begin(); it != v.end(); ++it){
        x += *it * exp;
        exp = -2*exp;
    }
    return x;
}

/*
 * sum rules:
 * 1 + 1 = 110
 * 11 + 1 = 0
 * 1 + 1 + 1 = 111
 *
 * carry values = 0, 1, 11
 * encoded as two bits --> 0, 1, 3 (b'00, b'01, b'11)
 *
 */
const int CARRY_0 = 0;
const int CARRY_1 = 1;
const int CARRY_11 = 2;

vector<int> sum(vector<int>& a, vector<int>& b){
    vector<int> v;

    while (a.size() < b.size())
        a.emplace_back();
    while (a.size() > b.size())
        b.emplace_back();

    int carry = CARRY_0;
    auto it_a = a.begin();
    // same size vectors, only check one for `end`
    for(auto it_b = b.begin(); it_b != b.end(); ++it_a, ++it_b){
        auto x = *it_a + *it_b;
        switch (x) {
            case 0:
                if (carry == CARRY_1) {
                    v.push_back(1);
                    carry = CARRY_0;
                } else if (carry == CARRY_11){
                    v.push_back(1);
                    carry = CARRY_1;
                } else {
                    v.push_back(0);
                    //carry = CARRY_0;
                }
                break;
            case 1:
                if (carry == CARRY_1) {
                    v.push_back(0);
                    carry = CARRY_11;
                } else if (carry == CARRY_11){
                    v.push_back(0);
                    carry = CARRY_0;
                } else {
                    v.push_back(1);
                    //carry = CARRY_0;
                }
                break;
            case 2:
                if (carry == CARRY_11){
                    v.push_back(1);
                    carry = CARRY_0;
                } else if (carry == CARRY_1){
                    v.push_back(1);
                    carry = CARRY_11;
                } else {
                    v.push_back(0);
                    carry = CARRY_11;
                }
                break;
        }
    }

    if (carry == CARRY_11) {
        v.push_back(1);
        v.push_back(1);
    } else if (carry == CARRY_1){
        v.push_back(1);
    }

    return v;
}

int main() {
    vector<int> v1{1,1,1,1,1,1};
    vector<int> v2{1,1,1};

    print(v1);
    cout << number(v1) << '\n';
    print(v2);
    cout << number(v2) << '\n';

    auto v3 = sum(v1,v2);
    print(v3);

    cout << number(v3) << '\n';

    return 0;
}
