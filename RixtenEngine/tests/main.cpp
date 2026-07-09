#include <cstdio>
#include "core/memory/DArr.h"

struct vec1 {
    int x;
    ~vec1() {
        printf("destructor vec1 have called\n");
    }
};

int main() {

    printf("-----This is testing build of engine-----\n");

    DArr<vec1> pool(10);

    pool.push_back({1580});
    
    printf("%d\n", pool[0].x);

    pool.delete_back();

}