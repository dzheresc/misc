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

int bit_count_hd(std::uint32_t v){ // Hacker's Delight
    v = (v & 0x55555555) + ((v>>1) & 0x55555555);
    v = (v & 0x33333333) + ((v>>2) & 0x33333333);
    v = (v & 0x0f0f0f0f) + ((v>>4) & 0x0f0f0f0f);
    v = (v & 0x00ff00ff) + ((v>>8) & 0x00ff00ff);
    v = (v & 0x0000ffff) + ((v>>16) & 0x0000ffff);
    return v;
}

int bit_count_hd_faster(std::uint32_t v){ // Hacker's Delight
    v = v - ((v>>1) & 0x55555555);
    v = (v & 0x33333333) + ((v>>2) & 0x33333333);
    v = (v + (v>>4)) & 0x0f0f0f0f;
    v = v + (v>>8);
    v = v + (v>>16);
    return v & 0x0000003f;
}

int main() {
    std::cout << bit_count(65535) << '\n';
    std::cout << bit_count_hd(65535) << '\n';
    std::cout << bit_count_hd_faster(65535) << '\n';
    return 0;
}
