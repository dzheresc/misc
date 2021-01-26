#include <iostream>
#include <unordered_map>
#include <vector>

/*
 *
 *
 */
class MM {

    struct block_header_t {
        int count;
        char pad[16-sizeof(int)];
    };

    struct data_header_t {
        char* base;
        char pad[16-sizeof(char*)];
    };

    const size_t LargeBlockSize = 64*1024;
    char* base;
    block_header_t* block_header;

    char* data;
    char* end;

    void reinitialize() {
        base = new char[LargeBlockSize];
        data = base;

        while(((long)data)&15) // align to 16 bytes (prob won't do anything)
            data++;

        block_header = (block_header_t*)data;
        block_header->count = 0;

        data += sizeof(block_header_t);
        end = base + LargeBlockSize;
    }

public:

    MM(){
        reinitialize();
    }

    void* alloc(size_t size);
    static void free(void* ptr);
};

void* MM::alloc(size_t size) {
    size = size + (16-size%16);

    if (data+size > end)
        reinitialize();

    block_header->count++;

    auto ptr = data + sizeof(data_header_t);
    ((data_header_t*)data)->base = base;
    data += size+sizeof(data_header_t);
    return ptr;
}

void MM::free(void* ptr){
    auto data_header_ptr = (data_header_t*)((char*)ptr - sizeof(data_header_t));
    if(--((block_header_t*)data_header_ptr->base)->count == 0)
        delete (block_header_t*)data_header_ptr->base;
}

int main() {
    MM mm;

    auto p = mm.alloc(33333);
    auto p1 = mm.alloc(31333);
    auto p2 = mm.alloc(3333);

    MM::free(p);
    MM::free(p1);
    MM::free(p2);
    return 0;
}
