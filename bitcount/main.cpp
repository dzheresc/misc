#include <iostream>

int bit_count(std::uint32_t v){
    int count{0};

    while(v){
        v = v&(v-1);
        ++count;
    }

    return count;
}

int z_count(std::uint32_t v){
    int count{0};

    while(v){
        v = v&(v-1);
        ++count;
    }

    return sizeof(v)*8 - count;
}

int main() {
    std::cout << bit_count(65535) << ' ' << z_count(5) << '\n';
    return 0;
}
