#pragma once
#include <cassert>
#include <cstddef>
#include <new>

#include "core/memory/MemoryArena.h"

template<typename T>
struct DArr {
    
    DArr(size_t maxElementsCount_);
    ~DArr();

    void push_back(const T& element);
    void set(uint32_t index, const T& element);
    void delete_back();

    void free();

    size_t size();
    T& back();
    bool empty();

    T& operator[](size_t index);

private:

    MemoryArena& arena;
    size_t maxElementsCount;
    size_t elementCount;
    size_t arenaOffset;

};

template <typename T>
inline DArr<T>::DArr(size_t maxElementsCount_) : maxElementsCount(maxElementsCount_),
    elementCount(0), arena(MemoryArena::GetInstance()) {
    arenaOffset = arena.allocate(sizeof(T) * maxElementsCount, alignof(T));
}

template <typename T>
inline DArr<T>::~DArr() {
    free();
}

template <typename T>
inline void DArr<T>::push_back(const T& element) {
    
    if(elementCount + 1 > maxElementsCount) {
        elementCount = 0;
    }

    void* ptr = arena.getPtr() + arenaOffset + (elementCount * sizeof(T));
    elementCount++;

    new(ptr) T(element); 
}

template <typename T>
inline void DArr<T>::set(uint32_t index, const T& element) {

    if(elementCount + 1 > maxElementsCount) {
        elementCount = 0;
    }

    if (index >= elementCount) {

        void* ptr = arena.getPtr() + arenaOffset + (sizeof(T) * index);
        elementCount++;

        new(ptr) T(element); 
    } else {
        (*this)[index] = element;
    }

}

template <typename T>
inline void DArr<T>::delete_back() {
    if(elementCount < 1) return;
    elementCount--;
    reinterpret_cast<T*>(arena.getPtr() + arenaOffset + (elementCount * sizeof(T)))->~T();
}

template <typename T>
inline void DArr<T>::free() {
    while(elementCount > 0) {
        elementCount--;
        reinterpret_cast<T*>(arena.getPtr() + arenaOffset + (elementCount * sizeof(T)))->~T();
    }

    arena.deallocateByOffset(arenaOffset);
}

template <typename T>
inline size_t DArr<T>::size() {
    return elementCount;
}

template <typename T>
inline T& DArr<T>::back() {
    return (*this)[elementCount];
}

template <typename T>
inline bool DArr<T>::empty() {
    return elementCount != 0;
}

template <typename T>
inline T& DArr<T>::operator[](size_t index) {
    return *reinterpret_cast<T*>(arena.getPtr() + arenaOffset + (sizeof(T) * index));
}