#include <iostream>
#include <vector>

using namespace std;

void swap(int& x, int& y){
    auto tmp = x;
    x = y;
    y = tmp;
}

int segregate(int *data, int n){
    int j = 0;
    for (int i = 0; i < n; ++i) {
        if (data[i] <= 0){
            swap(data[i], data[j]);
            j++;
        }
    }
    return j;
}

int findMissingPositive(int *v, int n) {
    for(int i=0;i < n;i++){
        if (abs(v[i])-1 < n && v[abs(v[i])-1] > 0){
            v[abs(v[i])-1] = -v[abs(v[i])-1];
        }
    }

    for(int i=0;i < n;i++){
        if (v[i] > 0)
            return i+1;
    }

    return n+1;
}

int findMissing(int *data, int n) {
    auto shift = segregate(data, n);
    return findMissingPositive(data+shift, n - shift);
}

int main() {
    vector<int> v{1,3,5,2,-1};
    // using pointer to internal array for simplicity
    cout << findMissing(v.data(), v.size()) << '\n';
    return 0;
}
